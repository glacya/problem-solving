"""이 도구 없이 예전에 푼 문제의 소스를 solution/ 에 모은다 (사용자가 직접 붙여넣거나 북마클릿으로 보낸다).

BOJ 의 소스(/source)와 채점 현황(/status)은 robots.txt 에서 막혀 있고 로그인이 필요해서 서버가 가져올 수 없다.
그래서 "푼 문제 중 solution/ 에 없는 것" 목록만 보여 주고, 소스와 푼 날짜는 사용자가 넣는다.
푼 문제 목록은 solved.ac 에서 받는다 (BOJ 의 AC 가 solved.ac 에 동기화된 것).
받는 데 몇 초 걸리므로 data/solved_cache.json 에 두고, '새로 불러오기' 때만 다시 받는다.
"소스가 없는 것"은 그 목록과 solution/boj/ 를 매번 비교해 구한다 (빠르다).
"""
import datetime
import json
import urllib.parse

from . import bojsource, db, gitops, history, solvedac, watcher
from .config import DATA, ROOT, SOURCE_LANGS, env

CACHE = DATA / "solved_cache.json"

MAX_CODE = 500_000


class BackfillError(Exception):
    def __init__(self, status, message):
        super().__init__(message)
        self.status = status


def local_files(judge="boj"):
    """{번호: [저장소 기준 경로, ...]}."""
    return gitops.solution_files(judge)


def _handle():
    handle = env("BOJ_HANDLE")
    if not handle:
        raise BackfillError(400, ".env 에 BOJ_HANDLE 을 설정하세요.")
    return handle


def solved_list(refresh=False):
    """(solved.ac 의 푼 문제 메타 목록, 받은 시각). 캐시가 없거나 핸들이 바뀌었거나 refresh 면 다시 받는다."""
    handle = _handle()
    if not refresh and CACHE.exists():
        try:
            c = json.loads(CACHE.read_text(encoding="utf-8"))
            if c.get("handle") == handle:
                return c["problems"], c["fetchedAt"]
        except (ValueError, KeyError):
            pass
    problems = solvedac.solved_problems(handle)
    c = {"handle": handle, "fetchedAt": db.now(), "problems": problems}
    DATA.mkdir(exist_ok=True)
    tmp = CACHE.with_suffix(".tmp")
    tmp.write_text(json.dumps(c, ensure_ascii=False), encoding="utf-8")
    tmp.replace(CACHE)
    return problems, c["fetchedAt"]


def status_url(pid):
    """BOJ 채점 현황에서 그 문제의 내 맞은 제출 (사용자가 브라우저로 연다)."""
    q = urllib.parse.urlencode({"problem_id": pid, "user_id": env("BOJ_HANDLE"), "result_id": 4})
    return f"https://www.acmicpc.net/status?{q}"


def missing(refresh=False):
    """푼 문제(solved.ac + 이 도구의 AC 기록) 중 solution/boj/ 에 소스가 없는 것."""
    handle = _handle()
    cached, fetched_at = solved_list(refresh)
    solved = {m["id"]: m for m in cached}
    for p in db.list_all():                         # solved.ac 에 아직 반영되지 않은 AC
        if p["status"] == "solved" and p["id"] not in solved:
            solved[p["id"]] = {k: p[k] for k in ("id", "title", "level", "tags", "solved_count")}
    have = local_files()
    rows = [m for pid, m in sorted(solved.items()) if pid not in have]
    return {"handle": handle, "solved": len(solved), "local": sum(1 for pid in solved if pid in have),
            "fetchedAt": fetched_at, "problems": rows}


def next_missing(after=0):
    """after 다음 번호의 소스 없는 문제 (없으면 처음부터). 모두 있으면 None."""
    rows = missing()["problems"]
    m = next((p for p in rows if p["id"] > after), rows[0] if rows else None)
    return m and {"id": m["id"], "title": m["title"], "statusUrl": status_url(m["id"]), "left": len(rows)}


def lookup(pid):
    """추가 화면에 보여 줄 문제 정보. 안 푼 문제면 티어와 태그를 빼고 보낸다 (스포일러 규칙)."""
    meta = solvedac.problem(pid)
    if meta is None:
        raise BackfillError(404, f"{pid} 는 solved.ac 에 없는 BOJ 문제입니다.")
    handle = env("BOJ_HANDLE")
    p = db.get(pid)
    solved = bool(p and p["status"] == "solved") or bool(handle and solvedac.solved_by(pid, handle))
    out = {"id": pid, "title": meta["title"], "solved": solved,
           "level": meta["level"] if solved else None, "tags": meta["tags"] if solved else None,
           "files": local_files().get(pid, []),
           "statusUrl": status_url(pid) if handle else None}
    return out


def _check_date(s):
    try:
        d = datetime.date.fromisoformat(s)
    except ValueError:
        raise BackfillError(400, "날짜는 YYYY-MM-DD 형식으로 넣으세요.") from None
    if d > datetime.date.today():
        raise BackfillError(400, "푼 날짜가 오늘보다 뒤입니다.")
    if d.year < 2000:
        raise BackfillError(400, "날짜를 확인하세요.")
    return d.isoformat()


def add(pid, date, lang, code):
    """소스를 solution/boj/<번호>.<확장자> 에 두고 AC 기록(푼 날짜)을 남긴다. 커밋은 하지 않는다.

    예전 기록이라 커밋 메시지는 사용자가 직접 쓴다. history/ 도 다시 써 두므로 함께 커밋하면 된다.
    """
    if lang not in SOURCE_LANGS:
        raise BackfillError(400, f"언어는 {', '.join(SOURCE_LANGS)} 중 하나")
    date = _check_date(date)
    code = code.replace("\r\n", "\n").replace("\r", "\n")
    if not code.strip():
        raise BackfillError(400, "코드가 비어 있습니다.")
    if len(code) > MAX_CODE:
        raise BackfillError(413, "코드가 너무 깁니다.")
    if not code.endswith("\n"):
        code += "\n"
    meta = _cached_meta(pid) or solvedac.problem(pid)
    if meta is None:
        raise BackfillError(404, f"{pid} 는 solved.ac 에 없는 BOJ 문제입니다.")

    with watcher.commit_lock:          # AC 확인 스레드가 history/ 를 쓰는 것과 겹치지 않게
        if local_files().get(pid):
            raise BackfillError(409, f"{pid} 의 소스가 이미 있습니다: {', '.join(local_files()[pid])}")
        fields = {k: meta[k] for k in ("title", "level", "tags", "solved_count")}
        if db.get(pid) is None:
            fields["source"] = "backfill"
        db.upsert(pid, language=lang, **fields)
        db.set_status(pid, "solved")
        p = db.upsert(pid, solved_at=date)
        rel = gitops.solution_rel(pid, lang)
        path = ROOT / rel
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text(watcher.with_header(code, lang, p), encoding="utf-8")
        hist = sorted(history.write())
    return {"id": pid, "path": rel, "history": f"history/{date[:7]}.md", "historyFiles": hist}


def _cached_meta(pid):
    try:
        return next((m for m in solved_list()[0] if m["id"] == pid), None)
    except (BackfillError, solvedac.ApiError):
        return None


def parse_page(page):
    """붙여넣은 BOJ 소스 페이지 해석 (저장하지 않는다)."""
    s = bojsource.parse(page or "")
    if s is None:
        raise BackfillError(400, "BOJ 소스 페이지로 읽을 수 없습니다.")
    s["warnings"] = _warnings(s)
    return s


def _warnings(s):
    w = []
    if not s["accepted"]:
        w.append(f"결과가 '{s['result']}' 입니다.")
    handle = env("BOJ_HANDLE")
    if handle and s["user"] and s["user"] != handle:
        w.append(f"{s['user']} 의 제출입니다 (내 핸들: {handle}).")
    if s["ext"] is None or s["ext"] not in SOURCE_LANGS:
        w.append(f"언어 '{s['language']}' 를 알 수 없습니다.")
    if not s["date"]:
        w.append("제출 날짜를 찾지 못했습니다.")
    return w


def from_page(page, confirm=False):
    """북마클릿: 소스 페이지를 해석해 바로 저장하고, 다음 문제를 알려 준다.

    {'saved': {...}} 또는 확인이 필요하면 {'confirm': [사유...]} (confirm=True 로 다시 보내면 저장).
    언어나 날짜를 모르면 저장할 수 없다.
    """
    s = parse_page(page)
    w = s["warnings"]
    if s["ext"] not in SOURCE_LANGS or not s["date"]:
        raise BackfillError(400, " ".join(w) + " 소스 추가 화면에서 직접 넣으세요.")
    if w and not confirm:
        return {"confirm": w, "problemId": s["problemId"]}
    try:
        saved = add(s["problemId"], s["date"], s["ext"], s["code"])
    except BackfillError as e:
        if e.status != 409:
            raise
        return {"exists": str(e), "next": next_missing(s["problemId"])}   # 이미 있으면 다음 문제로만 넘어간다
    saved.update({k: s[k] for k in ("submissionId", "problemTitle", "date", "language")})
    return {"saved": saved, "next": next_missing(s["problemId"])}

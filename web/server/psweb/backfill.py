"""이 도구 없이 예전에 푼 문제의 소스를 solution/ 에 모은다 (사용자가 직접 붙여넣는다).

BOJ 의 소스(/source)와 채점 현황(/status)은 robots.txt 에서 막혀 있고 로그인이 필요해서 서버가 가져올 수 없다.
그래서 "푼 문제 중 solution/ 에 없는 것" 목록만 보여 주고, 소스와 푼 날짜는 사용자가 넣는다.
푼 문제 목록은 solved.ac 에서 받는다 (BOJ 의 AC 가 solved.ac 에 동기화된 것).
"""
import datetime

from . import db, gitops, history, solvedac, watcher
from .config import ROOT, SOLUTIONS, SOURCE_LANGS, env

MAX_CODE = 500_000


class BackfillError(Exception):
    def __init__(self, status, message):
        super().__init__(message)
        self.status = status


def local_files(judge="boj"):
    """{번호: [solution/ 기준 경로, ...]}. 파일 이름이 <번호>.<확장자> 인 것만."""
    out = {}
    d = SOLUTIONS / judge
    if d.is_dir():
        for f in d.iterdir():
            if f.is_file() and f.stem.isdigit():
                out.setdefault(int(f.stem), []).append(gitops.solution_rel(int(f.stem), f.suffix[1:], judge))
    return out


def missing():
    """푼 문제(solved.ac + 이 도구의 AC 기록) 중 solution/boj/ 에 소스가 없는 것."""
    handle = env("BOJ_HANDLE")
    if not handle:
        raise BackfillError(400, ".env 에 BOJ_HANDLE 을 설정하세요.")
    solved = {m["id"]: m for m in solvedac.solved_problems(handle)}
    for p in db.list_all():                         # solved.ac 에 아직 반영되지 않은 AC
        if p["status"] == "solved" and p["id"] not in solved:
            solved[p["id"]] = {k: p[k] for k in ("id", "title", "level", "tags", "solved_count")}
    have = local_files()
    rows = [m for pid, m in sorted(solved.items()) if pid not in have]
    return {"handle": handle, "solved": len(solved), "local": sum(1 for pid in solved if pid in have),
            "problems": rows}


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
           "statusUrl": f"https://www.acmicpc.net/status?problem_id={pid}&user_id={handle}&result_id=4"
           if handle else None}
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
    meta = solvedac.problem(pid)
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

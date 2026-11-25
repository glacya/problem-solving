"""제출한 문제의 AC 를 서버가 확인한다 (브라우저를 닫아도, 서버를 재시작해도 이어서).

submit 할 때 DB 에 watch_until 을 적어 두면, 이 스레드가 그 시각까지 30초마다 solved.ac 를 확인하고
AC 가 보이면 제출본을 커밋 + push 한다. 상태는 DB 에만 있으므로 서버가 다시 뜨면 남은 것을 이어서 본다.
"""
import logging
import re
import threading
import time

from . import db, gitops, history, solvedac, workspace
from .config import SOURCE_LANGS, env

log = logging.getLogger("psweb.watcher")
INTERVAL_S = 30
WATCH_MINUTES = 15

commit_lock = threading.Lock()        # 요청('지금 확인', 소스 추가)과 스레드가 동시에 커밋하지 않게
_stop = threading.Event()


def with_header(code, lang, p):
    """solution/ 에 넣을 때 맨 위에 문제 정보 주석을 붙인다. 제출본(.submitted.*)은 그대로 둔다.

    Python 은 shebang 과 인코딩 선언이 첫 두 줄 안에 있어야 하므로 그 다음에 넣는다.
    """
    c = SOURCE_LANGS.get(lang, ("", "//"))[1]
    info = [
        f"{c} BOJ {p['id']} - {p['title']}",
        f"{c} 난이도: {history.tier_name(p['level'])}",
        f"{c} 푼 날짜: {p['solved_at'] or db.today()}",
        f"{c} https://www.acmicpc.net/problem/{p['id']}",
    ]
    lines = code.splitlines(keepends=True)
    keep = 0
    if lang == "py":
        while keep < min(2, len(lines)) and (
            (keep == 0 and lines[0].startswith("#!")) or re.match(r"^[ \t\f]*#.*?coding[:=]", lines[keep])
        ):
            keep += 1
    return "".join(lines[:keep]) + "\n".join(info) + "\n\n" + "".join(lines[keep:])


def confirm_ac(pid, manual=False, fallback_code=None):
    """AC 면 기록하고 제출본을 커밋한다. {'solved': bool, 'message'?, 'commit'?}.

    manual=True 는 사용자가 BOJ 에서 AC 를 직접 확인했다는 뜻이다. solved.ac 확인을 건너뛰고,
    제출본이 없으면 fallback_code(지금 코드의 번들)를 커밋한다.
    """
    handle = env("BOJ_HANDLE")
    if not handle and not manual:
        return {"solved": False, "message": ".env 에 BOJ_HANDLE 을 설정하세요."}
    with commit_lock:
        p = db.get(pid)
        if p is None:
            return {"solved": False, "message": f"{pid} 는 기록에 없습니다."}
        if p["status"] == "solved":
            return {"solved": True, "commit": {"ok": True, "log": p["commit_log"] or "이미 AC 로 기록됨"}}
        if not manual and not solvedac.solved_by(pid, handle):
            return {"solved": False, "message": f"아직 solved.ac 에 {handle} 의 AC 가 보이지 않습니다. "
                                                "BOJ 에서 AC 를 확인했다면 '직접 AC 처리' 를 누르세요."}

        try:
            meta = solvedac.problem(pid) or {}      # 티어/태그 (실패해도 AC 처리는 한다)
        except solvedac.ApiError:
            meta = {}
        db.upsert(pid, **{k: meta[k] for k in ("level", "tags", "solved_count") if k in meta})
        p = db.set_status(pid, "solved")
        snap = workspace.pdir(pid) / f".submitted.{p['language']}"
        source = snap.read_text(encoding="utf-8") if snap.exists() else fallback_code
        if source is not None:
            files = {gitops.solution_rel(pid, p["language"]): with_header(source, p["language"], p)}
            # 그 달의 기록만 같은 커밋에 (다른 달은 '소스 모으기' 로 바뀌었을 수 있고, 사용자가 직접 커밋한다)
            files.update(history.build_month(p["solved_at"][:7]))
            commit = gitops.commit_files(files, gitops.commit_message(p["solved_at"], pid))
        else:
            commit = {"ok": False, "log": "제출본이 없어 커밋하지 않았습니다."}
        db.upsert(pid, commit_log=("" if commit["ok"] else "실패: ") + commit["log"])
        log.info("AC %s: %s", pid, commit["log"])
        return {"solved": True, "commit": commit}


def _cleanup():
    from . import cleanup                 # cleanup → backfill → watcher 순환 import 를 피한다
    try:
        with commit_lock:                 # 기록 페이지의 '정리' 요청과 겹치지 않게
            cleanup.run()
    except Exception:
        log.exception("풀이 폴더 정리 실패")


def _loop():
    _cleanup()                            # 서버가 뜰 때 한 번, 그 뒤 몇 시간마다 (지울 것이 없으면 아무 일도 없다)
    last_cleanup = time.monotonic()
    while not _stop.wait(INTERVAL_S):
        if time.monotonic() - last_cleanup > 3600 * 6:
            _cleanup()
            last_cleanup = time.monotonic()
        for p in db.watching():
            try:
                confirm_ac(p["id"])
            except Exception:          # 네트워크 오류 등은 다음 차례에 다시 본다
                log.exception("AC 확인 실패: %s", p["id"])


def start():
    _stop.clear()
    threading.Thread(target=_loop, name="ac-watcher", daemon=True).start()


def stop():
    _stop.set()

"""풀이 폴더(problems/<번호>/) 정리: AC 를 받고 마지막 제출일로부터 KEEP_DAYS 일 이상 지난 문제의 폴더를 지운다.

problems/ 는 커밋하지 않지만 지문, 예제, 채점 출력, 빌드 결과가 쌓여 작업 공간이 커지므로 비운다.
- 제출본이 solution/ 에 있는 문제만 지운다 (커밋이 실패해 코드가 solution/ 에 없으면 남긴다).
- 메모(memo.md)는 남긴다. 메모가 없으면 폴더째 지운다.
서버가 뜰 때와 그 뒤 CHECK_HOURS 시간마다 watcher 스레드가 부른다. 기록 페이지에서도 바로 정리할 수 있다.
"""
import datetime
import logging
import shutil

from . import backfill, db, workspace
from .config import PROBLEMS

log = logging.getLogger("psweb.cleanup")
KEEP_DAYS = 3
CHECK_HOURS = 6
KEEP_FILES = {"memo.md"}


def _last_date(p):
    """마지막 제출일. 제출본 없이 '직접 AC 처리' 했다면 푼 날짜."""
    return (p["submitted_at"] or p["solved_at"] or "")[:10] or None


def candidates(today=None):
    today = today or datetime.date.today()
    have = backfill.local_files()
    out = []
    if not PROBLEMS.is_dir():
        return out
    for d in PROBLEMS.iterdir():
        if not (d.is_dir() and d.name.isdigit()):
            continue
        pid = int(d.name)
        p = db.get(pid)
        if p is None or p["status"] != "solved" or pid not in have:
            continue
        last = _last_date(p)
        if last is None or (today - datetime.date.fromisoformat(last)).days < KEEP_DAYS:
            continue
        if all(f.name in KEEP_FILES for f in d.iterdir()):
            continue                     # 이미 정리됨 (메모만 남음)
        out.append({"id": pid, "title": p["title"], "last": last})
    return sorted(out, key=lambda c: c["id"])


def run(today=None):
    """정리하고, 정리한 문제 목록을 돌려준다."""
    done = []
    for c in candidates(today):
        d = workspace.pdir(c["id"])
        for f in d.iterdir():
            if f.name in KEEP_FILES:
                continue
            if f.is_dir() and not f.is_symlink():
                shutil.rmtree(f)
            else:
                f.unlink()
        if not any(d.iterdir()):
            d.rmdir()
        done.append(c)
    if done:
        log.info("풀이 폴더 정리: %s", ", ".join(str(c["id"]) for c in done))
    return done

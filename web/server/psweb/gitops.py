"""AC 받은 제출본(solution/<저지>/)과 월별 기록(history/)만 커밋 + push 한다.

커밋 메시지: "YYYY-MM-DD: BOJ {번호}" (푼 날짜). 다른 스테이징된 변경은 섞지 않는다.
"""
import os
import subprocess

from .config import ROOT, SOLUTIONS

# 서버에는 터미널이 없으므로 인증을 묻지 말고 바로 실패하게 한다 (credential helper / PAT 를 미리 설정).
_ENV = {**os.environ, "GIT_TERMINAL_PROMPT": "0"}


def _git(*args):
    r = subprocess.run(["git", *args], cwd=ROOT, capture_output=True, text=True, timeout=120, env=_ENV)
    return r.returncode, (r.stdout + r.stderr).strip()


def solution_rel(pid, ext, judge="boj"):
    """저장소 루트 기준 경로. 저지마다 폴더를 나눈다 (나중에 cf 등을 붙일 수 있게)."""
    return f"{SOLUTIONS.name}/{judge}/{pid}.{ext}"


def commit_message(date, pid, judge="boj"):
    return f"{date}: {judge.upper()} {pid}"


def commit_files(files, message, push=True):
    """files: {저장소 기준 경로: 내용}. 이 파일들만 커밋한다."""
    for rel, text in files.items():
        path = ROOT / rel
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text(text, encoding="utf-8")
    rels = list(files)
    log = []
    rc, out = _git("add", "--", *rels)
    if rc:
        return {"ok": False, "log": out}
    rc, _ = _git("diff", "--cached", "--quiet", "--", *rels)
    if rc == 0:
        log.append("이미 같은 내용으로 커밋되어 있습니다.")
    else:
        rc, out = _git("commit", "-q", "-m", message, "--", *rels)   # 다른 스테이징된 변경은 섞지 않는다
        if rc:
            return {"ok": False, "log": out}
        log.append(f"커밋: {message}")
    if push:
        rc, _ = _git("rev-parse", "--abbrev-ref", "--symbolic-full-name", "@{u}")
        rc, out = _git("push", "-q") if rc == 0 else _git("push", "-q", "-u", "origin", "HEAD")
        if rc:
            return {"ok": False, "log": "\n".join(log + [f"push 실패: {out}"])}
        log.append("push 완료")
    return {"ok": True, "log": "\n".join(log)}

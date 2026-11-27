"""AC 받은 제출본(solution/<저지>/)과 월별 기록(history/)만 커밋한다. push 는 하지 않는다 (사용자가 직접 한다).

커밋 메시지: "YYYY-MM-DD: BOJ {번호}" (푼 날짜). 다른 스테이징된 변경은 섞지 않는다.
"""
import itertools
import os
import subprocess

from .config import ROOT, SOLUTIONS

# 서버에는 터미널이 없으므로 인증을 묻지 말고 바로 실패하게 한다 (credential helper / PAT 를 미리 설정).
_ENV = {**os.environ, "GIT_TERMINAL_PROMPT": "0"}


def _git(*args):
    r = subprocess.run(["git", *args], cwd=ROOT, capture_output=True, text=True, timeout=120, env=_ENV)
    return r.returncode, (r.stdout + r.stderr).strip()


def bucket(n, size):
    """번호가 든 폴더 이름: bucket(2345, 100) → "2300-2399"."""
    lo = n // size * size
    return f"{lo}-{lo + size - 1}"


def solution_rel(pid, ext, judge="boj"):
    """저장소 루트 기준 경로. 저지마다 폴더를 나누고, 한 폴더에 100개쯤 되도록 1000 단위 / 100 단위로 나눈다.

    solution/boj/2000-2999/2300-2399/2345.cpp. 숫자가 아닌 번호(예: CF 의 1900A)는 앞의 숫자로 나눈다.
    """
    digits = "".join(itertools.takewhile(str.isdigit, str(pid)))
    if not digits:
        return f"{SOLUTIONS.name}/{judge}/{pid}.{ext}"
    n = int(digits)
    return f"{SOLUTIONS.name}/{judge}/{bucket(n, 1000)}/{bucket(n, 100)}/{pid}.{ext}"


def solution_files(judge="boj"):
    """{번호: [저장소 기준 경로, ...]}. 폴더 깊이와 상관없이 <번호>.<확장자> 파일을 모두 찾는다."""
    out = {}
    d = SOLUTIONS / judge
    if d.is_dir():
        for f in sorted(d.rglob("*")):
            if f.is_file() and f.stem.isdigit():
                out.setdefault(int(f.stem), []).append(f.relative_to(ROOT).as_posix())
    return out


def migrate_layout(judge="boj"):
    """solution/<저지>/ 의 파일을 solution_rel 의 폴더 구조로 옮긴다 (서버가 뜰 때마다, 옮길 것이 없으면 아무 일도 없다).

    커밋된 파일은 git mv 로 옮겨 이름 바꾸기로 스테이징하고, 커밋하지 않은 파일은 그냥 옮긴다. 커밋은 하지 않는다.
    """
    moved = []
    for pid, rels in solution_files(judge).items():
        for rel in rels:
            target = solution_rel(pid, rel.rsplit(".", 1)[-1], judge)
            if rel == target or (ROOT / target).exists():
                continue
            (ROOT / target).parent.mkdir(parents=True, exist_ok=True)
            tracked = _git("ls-files", "--error-unmatch", "--", rel)[0] == 0
            if not (tracked and _git("mv", "--", rel, target)[0] == 0):
                (ROOT / rel).rename(ROOT / target)
            moved.append((rel, target))
    for d in sorted((SOLUTIONS / judge).rglob("*"), reverse=True):   # 빈 폴더 정리
        if d.is_dir() and not any(d.iterdir()):
            d.rmdir()
    return moved


def commit_message(date, pid, judge="boj"):
    return f"{date}: {judge.upper()} {pid}"


def commit_files(files, message):
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
    return {"ok": True, "log": "\n".join(log)}

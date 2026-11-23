"""경로와 설정 (.env)."""
import os
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]          # 저장소 루트
PROBLEMS = ROOT / "problems"                          # 문제별 작업 폴더 (VSCode 에서 코딩)
LIB = ROOT / "lib"                                    # 자주 쓰는 코드 (#include / import 로 가져다 쓴다)
SOLUTIONS = ROOT / "solution"                         # AC 받은 제출본 (커밋 대상)
DATA = ROOT / "data"                                  # SQLite 등 로컬 상태
CLIENT_DIST = ROOT / "web" / "client" / "dist"
TEMPLATES = Path(__file__).resolve().parent / "templates"

LANGS = {
    "cpp": {"label": "C++20", "file": "main.cpp", "boj": "C++20"},
    "py": {"label": "Python 3", "file": "main.py", "boj": "Python 3 / PyPy3"},
}


def _read_env_file():
    out = {}
    path = ROOT / ".env"
    if path.exists():
        for line in path.read_text(encoding="utf-8").splitlines():
            line = line.strip()
            if line and not line.startswith("#") and "=" in line:
                k, v = line.split("=", 1)
                out[k.strip()] = v.strip().strip("'\"")
    return out


def env(key, default=""):
    """환경 변수 → .env 순. .env 는 매번 다시 읽어 서버를 재시작하지 않아도 반영된다."""
    return os.environ.get(key) or _read_env_file().get(key) or default


def normalize_query(q):
    """뽑기 쿼리에 BOJ 한정(oj:boj)과 내가 푼 문제 제외(-@핸들)를 붙인다.

    solved.ac 는 BOJ 외의 저지(DOJ, JUNGOL 등)도 다루므로 oj:boj 가 없으면 다른 저지 문제가 섞인다.
    """
    q = q.strip()
    if "oj:" not in q:
        q += " oj:boj"
    handle = env("BOJ_HANDLE")
    if handle and "-@" not in q:
        q += f" -@{handle}"
    return q.strip()


def default_query():
    return normalize_query(env("SOLVEDAC_QUERY", "*g"))

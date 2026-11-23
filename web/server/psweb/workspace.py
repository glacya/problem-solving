"""문제 작업 폴더 (problems/<번호>/) 와 VSCode 연동."""
import os
import shutil
import subprocess
from pathlib import Path

from .config import LANGS, PROBLEMS, TEMPLATES


def pdir(pid) -> Path:
    return PROBLEMS / str(pid)


def source(pid, lang) -> Path:
    return pdir(pid) / LANGS[lang]["file"]


def ensure_source(pid, lang):
    """풀이 파일이 없으면 템플릿으로 만든다. 이미 있으면 건드리지 않는다."""
    p = source(pid, lang)
    if not p.exists():
        p.parent.mkdir(parents=True, exist_ok=True)
        shutil.copy(TEMPLATES / LANGS[lang]["file"], p)
    return p


def memo_path(pid) -> Path:
    return pdir(pid) / "memo.md"


def read_memo(pid):
    """(본문, 수정 시각 ISO 또는 None). VSCode 에서 고쳐도 반영된다."""
    p = memo_path(pid)
    if not p.exists():
        return "", None
    import datetime
    mtime = datetime.datetime.fromtimestamp(p.stat().st_mtime).isoformat(timespec="seconds")
    return p.read_text(encoding="utf-8"), mtime


def write_memo(pid, text):
    p = memo_path(pid)
    if not text.strip():
        p.unlink(missing_ok=True)          # 빈 메모는 파일을 남기지 않는다
        return read_memo(pid)
    p.parent.mkdir(parents=True, exist_ok=True)
    tmp = p.with_suffix(".md.tmp")
    tmp.write_text(text, encoding="utf-8")
    tmp.replace(p)                         # 쓰다가 꺼져도 반쯤 쓴 파일이 남지 않게
    return read_memo(pid)


def memo_excerpt(pid, n=60):
    p = memo_path(pid)
    if not p.exists():
        return ""
    first = next((ln.strip() for ln in p.read_text(encoding="utf-8").splitlines() if ln.strip()), "")
    return first[:n] + ("…" if len(first) > n else "")


def existing_langs(pid):
    return [lang for lang in LANGS if source(pid, lang).exists()]


def tests(pid):
    d = pdir(pid)
    names = sorted(p.stem for p in (d / "inputs").glob("*.in")) if (d / "inputs").is_dir() else []
    return [{"name": n, "hasOutput": (d / "outputs" / f"{n}.out").exists()} for n in names]


def editor_uri(path: Path):
    """브라우저에서 누르면 VSCode 가 여는 링크. WSL 이면 원격 경로로."""
    distro = os.environ.get("WSL_DISTRO_NAME")
    if distro:
        return f"vscode://vscode-remote/wsl+{distro}{path}"
    return f"vscode://file{path}"


def open_in_editor(path: Path):
    """VSCode 터미널에서 서버를 띄웠다면 `code` 로 바로 연다. 아니면 False (페이지가 링크로 연다)."""
    if not os.environ.get("VSCODE_IPC_HOOK_CLI"):
        return False
    code = next((c for c in os.environ.get("PATH", "").split(os.pathsep)
                 if (Path(c) / "code").is_file() and "remote-cli" in c), None)
    if not code:
        return False
    r = subprocess.run([str(Path(code) / "code"), "-r", "-g", str(path)], capture_output=True, timeout=15)
    return r.returncode == 0

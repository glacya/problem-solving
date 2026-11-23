"""번들한 풀이를 문제 폴더의 inputs/ 에 돌려 채점한다.

판정: PASS · WA · RE · TLE · CE · SKIP(정답 파일 없음. 런타임 에러와 강제 종료는 정답이 없어도 실패)
비교: 줄 끝 공백/탭 1개, 파일 끝 빈 줄 1개, 마지막 개행 유무, CRLF 는 무시한다 (대부분의 OJ 와 같이).
시간: 시간 제한을 넘기면 TLE, 제한의 5배(최소 5초)에서 강제 종료한다.
"""
import os
import re
import shutil
import signal
import subprocess
import sys
import tempfile
import threading
import time
from pathlib import Path


def _lines(text):
    ls = text.replace("\r\n", "\n").split("\n")
    if ls and ls[-1] == "":
        ls.pop()
    if ls and ls[-1] == "":       # 파일 끝 빈 줄 1개 허용
        ls.pop()
    return ls


def _norm(line):
    return line[:-1] if line.endswith((" ", "\t")) else line     # 줄 끝 공백 1개 허용


def compare(expected, got):
    """같으면 None, 다르면 첫 차이 설명."""
    e, g = _lines(expected), _lines(got)
    if list(map(_norm, e)) == list(map(_norm, g)):
        return None
    for i in range(max(len(e), len(g))):
        if i >= len(e):
            return f"{i + 1}번째 줄: 출력이 더 김 (정답 {len(e)}줄, 출력 {len(g)}줄)"
        if i >= len(g):
            return f"{i + 1}번째 줄: 출력이 일찍 끝남 (정답 {len(e)}줄, 출력 {len(g)}줄)"
        if _norm(e[i]) != _norm(g[i]):
            return f"{i + 1}번째 줄: 정답 {e[i]!r}, 출력 {g[i]!r}"
    return "알 수 없는 차이"


def _order(name):
    group = 0 if name.startswith("sample") else 1 if name.startswith("auto") else 2
    num = re.search(r"\d+", name)
    return group, re.sub(r"\d+", "", name), int(num.group()) if num else -1, name


TIME_BIN = "/usr/bin/time" if os.path.exists("/usr/bin/time") else None


def _exit_reason(rc):
    """GNU time 은 시그널로 죽은 자식을 128+N 으로, 직접 실행은 -N 으로 알려준다."""
    sig = -rc if rc < 0 else rc - 128 if TIME_BIN and rc > 128 else 0
    if sig:
        try:
            return f"{signal.Signals(sig).name} (exit {rc})"
        except ValueError:
            pass
    return f"exit code {rc}"


def _run(cmd, in_path, out_path, kill_s):
    """(종료 코드 또는 'timeout', ms, 최대 메모리 KB, stderr).

    메모리는 GNU time 으로 잰다. 이 프로세스에서 바로 잰 rusage 는 exec 직전의
    (fork 된 Python) 메모리까지 포함해 실제보다 크게 나온다.
    """
    with tempfile.TemporaryDirectory() as tmp, open(in_path, "rb") as fin, open(out_path, "wb") as fout, \
            open(os.path.join(tmp, "err"), "w+b") as ferr:
        mem_file = os.path.join(tmp, "mem")
        full = [TIME_BIN, "-f", "%M", "-o", mem_file, *cmd] if TIME_BIN else cmd
        t0 = time.monotonic()
        p = subprocess.Popen(full, stdin=fin, stdout=fout, stderr=ferr, start_new_session=True)
        killed = threading.Event()

        def kill():
            killed.set()
            try:
                os.killpg(p.pid, signal.SIGKILL)       # time 과 풀이 프로세스를 함께 끝낸다
            except ProcessLookupError:
                pass

        timer = threading.Timer(kill_s, kill)
        timer.start()
        _, status, usage = os.wait4(p.pid, 0)
        timer.cancel()
        ms = int((time.monotonic() - t0) * 1000)
        p.returncode = rc = os.waitstatus_to_exitcode(status)
        mem = usage.ru_maxrss
        if TIME_BIN:
            try:
                mem = int(open(mem_file).read().split()[-1])
            except (OSError, ValueError, IndexError):
                pass
            # GNU time 은 자식이 시그널로 죽으면 "Command terminated by signal N" 을 출력하고 128+N 으로 끝난다
            ferr.seek(0)
            err = ferr.read(8000).decode(errors="replace")
            err = "\n".join(ln for ln in err.splitlines() if not ln.startswith("Command "))
        else:
            ferr.seek(0)
            err = ferr.read(4000).decode(errors="replace")
    return ("timeout" if killed.is_set() else rc), ms, mem, err[:4000]


def judge(pdir: Path, code: str, lang: str, tl_ms: int):
    inputs, outputs, answers = pdir / "inputs", pdir / "outputs", pdir / "answers"
    answers.mkdir(exist_ok=True)
    kill_s = max(5.0, tl_ms * 5 / 1000)
    work = Path(tempfile.mkdtemp())
    res = {"lang": lang, "limitMs": tl_ms, "results": []}
    try:
        if lang == "cpp":
            (work / "main.cpp").write_text(code, encoding="utf-8")
            t0 = time.monotonic()
            c = subprocess.run(["g++", "-std=c++20", "-O2", "-o", str(work / "main"), str(work / "main.cpp")],
                               capture_output=True, text=True)
            res["compile"] = {"ok": c.returncode == 0, "ms": int((time.monotonic() - t0) * 1000),
                              "log": c.stderr[-8000:]}
            cmd = [str(work / "main")]
        else:
            (work / "main.py").write_text(code, encoding="utf-8")
            c = subprocess.run([sys.executable, "-m", "py_compile", str(work / "main.py")], capture_output=True, text=True)
            res["compile"] = {"ok": c.returncode == 0, "ms": 0, "log": c.stderr[-8000:]}
            cmd = [os.environ.get("PS_PYTHON", shutil.which("python3") or sys.executable), str(work / "main.py")]
        if not res["compile"]["ok"]:
            return res

        names = sorted((p.stem for p in inputs.glob("*.in")), key=_order) if inputs.is_dir() else []
        for name in names:
            exp_path, out_path = outputs / f"{name}.out", answers / f"{name}.out"
            rc, ms, mem, err = _run(cmd, inputs / f"{name}.in", out_path, kill_s)
            reason, status = "", "PASS"
            if rc == "timeout":
                status, reason = "TLE", f"{kill_s:g}초 안에 끝나지 않아 강제 종료"
            elif rc != 0:
                status, reason = "RE", _exit_reason(rc)
            elif not exp_path.exists():
                status = "SKIP"
            elif msg := compare(exp_path.read_text(encoding="utf-8", errors="replace"),
                                out_path.read_text(encoding="utf-8", errors="replace")):
                status, reason = "WA", msg
            elif ms > tl_ms:
                status, reason = "TLE", f"{ms} ms > 제한 {tl_ms} ms (출력은 정답)"
            res["results"].append({
                "name": name, "status": status, "ms": ms, "memKb": mem, "reason": reason,
                "input": (inputs / f"{name}.in").read_text(encoding="utf-8", errors="replace")[:4000],
                "expected": exp_path.read_text(encoding="utf-8", errors="replace")[:4000] if exp_path.exists() else None,
                "output": out_path.read_text(encoding="utf-8", errors="replace")[:4000],
                "stderr": err,
            })
    finally:
        shutil.rmtree(work, ignore_errors=True)
    return res

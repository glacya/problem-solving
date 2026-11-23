"""풀이 파일을 BOJ 에 제출할 수 있는 단일 파일로 만든다.

C++   : #include "x" 를 재귀적으로 펼친다 (문제 폴더 → lib/ 순서로 찾고, 같은 파일은 한 번만).
        #include <x> 는 중복 없이 맨 위로 모으고, #pragma once 는 지운다. 주석 안의 include 는 그대로 둔다.
Python: 문제 폴더나 lib/ 에 있는 모듈을 import 하면, 그 모듈들을 파일 앞에서 sys.modules 에 등록한다.
        원래의 import 줄은 그대로 동작하므로 코드는 바꾸지 않는다.
"""
import ast
import re
from pathlib import Path

from .config import LIB


class BundleError(Exception):
    pass


# ---- C++ ----

_INC_LOCAL = re.compile(r'^\s*#\s*include\s*"([^"]+)"')
_INC_SYS = re.compile(r"^\s*#\s*include\s*(<[^>]+>)")
_PRAGMA_ONCE = re.compile(r"^\s*#\s*pragma\s+once\b")


def _strip_comments_state(line, in_block):
    """(주석을 뺀 코드 부분, 줄 끝에서의 블록 주석 상태)."""
    code, i = "", 0
    while i < len(line):
        if in_block:
            j = line.find("*/", i)
            if j < 0:
                return code, True
            i, in_block = j + 2, False
        elif line.startswith("//", i):
            break
        elif line.startswith("/*", i):
            in_block, i = True, i + 2
        else:
            code += line[i]
            i += 1
    return code, in_block


def bundle_cpp(src: Path):
    seen, sys_headers, out, used = set(), [], [], []

    def resolve(base: Path, name):
        for d in (base, src.parent, LIB):
            p = (d / name).resolve()
            if p.is_file():
                return p
        raise BundleError(f'include 를 찾을 수 없음: "{name}" ({base})')

    def expand(path: Path, top):
        seen.add(path)
        if not top:
            out.append(f"// ===== begin {path.name} =====")
            used.append(str(path))
        in_block = False
        for line in path.read_text(encoding="utf-8").splitlines():
            code, in_block_next = _strip_comments_state(line, in_block)
            was_block, in_block = in_block, in_block_next
            if not was_block:
                if _PRAGMA_ONCE.match(code):
                    continue
                if m := _INC_LOCAL.match(code):
                    p = resolve(path.parent, m.group(1))
                    if p not in seen:
                        expand(p, False)
                    continue
                if m := _INC_SYS.match(code):
                    if m.group(1) not in sys_headers:
                        sys_headers.append(m.group(1))
                    continue
            out.append(line)
        if not top:
            out.append(f"// ===== end {path.name} =====")

    expand(src.resolve(), True)
    body = re.sub(r"\n{3,}", "\n\n", "\n".join(out)).strip("\n")
    return "\n".join(f"#include {h}" for h in sys_headers) + "\n\n" + body + "\n", used


# ---- Python ----

def _local_imports(path: Path, roots):
    """path 가 import 하는 로컬 모듈 이름 → 파일."""
    try:
        tree = ast.parse(path.read_text(encoding="utf-8"))
    except SyntaxError as e:
        raise BundleError(f"{path.name}: 문법 오류 {e.lineno}행: {e.msg}") from None
    names = []
    for node in ast.walk(tree):
        if isinstance(node, ast.Import):
            names += [a.name.split(".")[0] for a in node.names]
        elif isinstance(node, ast.ImportFrom) and node.level == 0 and node.module:
            names.append(node.module.split(".")[0])
    found = {}
    for n in names:
        for r in roots:
            p = r / f"{n}.py"
            if p.is_file():
                found[n] = p.resolve()
                break
    return found


def bundle_py(src: Path):
    roots = [src.parent, LIB]
    order, visiting = [], set()

    def visit(name, path):
        if name in visiting or any(n == name for n, _ in order):
            return
        visiting.add(name)
        for dep, p in _local_imports(path, roots).items():
            visit(dep, p)
        order.append((name, path))

    for name, path in _local_imports(src, roots).items():
        visit(name, path)

    code = src.read_text(encoding="utf-8")
    if not order:
        return code if code.endswith("\n") else code + "\n", []
    head = ["# ===== bundled local modules =====",
            "import sys as _sys, types as _types",
            "def _bundle_load(_name, _src):",
            "    _m = _types.ModuleType(_name)",
            "    _sys.modules[_name] = _m",
            "    exec(compile(_src, _name + '.py', 'exec'), _m.__dict__)"]
    for name, path in order:
        head.append(f"_bundle_load({name!r}, {path.read_text(encoding='utf-8')!r})")
    head.append("# ===== end bundled modules =====\n")
    return "\n".join(head) + "\n" + code + ("" if code.endswith("\n") else "\n"), [str(p) for _, p in order]


def bundle(src: Path, lang):
    if not src.exists():
        raise BundleError(f"{src.name} 이 없습니다.")
    return bundle_cpp(src) if lang == "cpp" else bundle_py(src)

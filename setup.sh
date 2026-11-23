#!/usr/bin/env bash
# 웹 페이지(ps.sh)에 필요한 것을 준비한다. 여러 번 실행해도 된다.
#
#   ./setup.sh            .venv 생성 + 백엔드 패키지 설치 + 프론트엔드 빌드
#   ./setup.sh --no-web   .venv 만 준비
#
#   - Python: venv/pip 가 되는 python3 를 찾아 .venv 를 만든다 (PYTHON 으로 지정 가능).
#   - Node: PATH 에 없으면 .tools/node 에 공식 배포본을 받는다 (전역 설정은 건드리지 않는다).
#     평소 실행(ps.sh)에는 Node 가 필요 없고, 빌드할 때만 쓴다.

set -euo pipefail

root="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
NODE_VERSION="v26.13.1"

# ---- Python venv ----
find_python() {
    local c
    for c in "${PYTHON:-}" "$(pyenv which python3 2>/dev/null || true)" \
             "$HOME"/.pyenv/versions/3.1[2-9]*/bin/python3 python3; do
        [[ -n "$c" ]] || continue
        command -v "$c" > /dev/null 2>&1 || [[ -x "$c" ]] || continue
        if "$c" -c 'import venv, ensurepip, sys; sys.exit(sys.version_info < (3, 11))' 2> /dev/null; then
            echo "$c"
            return 0
        fi
    done
    return 1
}

if [[ ! -x "$root/.venv/bin/python" ]]; then
    py="$(find_python)" || { echo "venv 와 pip 를 쓸 수 있는 Python 3.11+ 가 없습니다. PYTHON=경로 로 지정하세요." >&2; exit 1; }
    echo "venv 생성: $py"
    "$py" -m venv "$root/.venv"
fi
"$root/.venv/bin/python" -m pip install -q --upgrade pip
"$root/.venv/bin/python" -m pip install -q -r "$root/web/server/requirements.txt"
echo "백엔드 패키지 설치 완료 (.venv)"

[[ "${1:-}" == "--no-web" ]] && exit 0

# ---- Node ----
if command -v node > /dev/null && [[ "$(command -v node)" != /mnt/* ]]; then
    node_bin="$(dirname "$(command -v node)")"
else
    node_dir="$root/.tools/node"
    if [[ ! -x "$node_dir/bin/node" || "$("$node_dir/bin/node" --version)" != "$NODE_VERSION" ]]; then
        echo "Node $NODE_VERSION 받는 중 (.tools/node)"
        tmp="$(mktemp -d)"
        trap 'rm -rf "$tmp"' EXIT
        name="node-$NODE_VERSION-linux-x64.tar.xz"
        curl -fsSL -o "$tmp/$name" "https://nodejs.org/dist/$NODE_VERSION/$name"
        curl -fsSL -o "$tmp/SHASUMS256.txt" "https://nodejs.org/dist/$NODE_VERSION/SHASUMS256.txt"
        (cd "$tmp" && grep " $name\$" SHASUMS256.txt | sha256sum -c --quiet -)
        rm -rf "$node_dir"
        mkdir -p "$node_dir"
        tar -xJf "$tmp/$name" -C "$node_dir" --strip-components=1
    fi
    node_bin="$node_dir/bin"
fi
export PATH="$node_bin:$PATH"
echo "Node $(node --version) ($node_bin)"

cd "$root/web/client"
if [[ -f package-lock.json ]]; then npm ci --no-audit --no-fund; else npm install --no-audit --no-fund; fi
npm run build
echo "프론트엔드 빌드 완료 (web/client/dist)"

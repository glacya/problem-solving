#!/usr/bin/env bash
# PS 웹 페이지 서버. 터미널을 닫아도 계속 돌도록 백그라운드로 띄운다.
#
#   ./ps.sh             서버가 없으면 백그라운드로 띄우고, 브라우저로 연다
#   ./ps.sh start       띄우기만 (브라우저 X)
#   ./ps.sh stop        끄기
#   ./ps.sh restart     다시 띄우기 (백엔드 코드를 고쳤을 때)
#   ./ps.sh status      실행 중인지
#   ./ps.sh logs        로그 보기 (data/server.log, Ctrl+C 로 나감)
#   ./ps.sh fg          포그라운드로 실행 (디버깅)
#   ./ps.sh dev         백엔드(--reload) + Vite 개발 서버(5173)
#
#   주소: http://127.0.0.1:8710 (PS_PORT 로 바꿀 수 있다)
#   서버의 상태(기록, 메모, AC 대기)는 data/ 와 problems/ 에 있으므로 언제 껐다 켜도 이어진다.
#   VSCode 터미널에서 띄우면 '풀기' 가 그 VSCode 에서 바로 파일을 연다.

set -euo pipefail

root="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
port="${PS_PORT:-8710}"
url="http://127.0.0.1:$port/"
pidfile="$root/data/server.$port.pid"     # 포트별로 따로 둔다 (테스트용 서버가 실제 서버의 PID 를 덮지 않게)
logfile="$root/data/server.log"
py="$root/.venv/bin/python"
uvicorn=("$py" -m uvicorn psweb.app:app --app-dir "$root/web/server" --host 127.0.0.1 --port "$port")

open_url() {
    if command -v wslview > /dev/null; then wslview "$1"
    elif command -v explorer.exe > /dev/null; then explorer.exe "$1" || true
    elif command -v xdg-open > /dev/null; then xdg-open "$1" > /dev/null 2>&1
    elif command -v open > /dev/null; then open "$1"
    else echo "브라우저로 여세요: $1"
    fi
}

healthy() { curl -sf -m 2 -o /dev/null "${url}api/health"; }

running_pid() {
    [[ -f "$pidfile" ]] || return 1
    local pid
    pid="$(cat "$pidfile")"
    kill -0 "$pid" 2> /dev/null && echo "$pid"
}

start() {
    if healthy; then
        echo "실행 중: $url"
        return 0
    fi
    [[ -x "$py" ]] || { echo ".venv 가 없습니다. ./setup.sh 를 먼저 실행하세요." >&2; exit 1; }
    [[ -f "$root/web/client/dist/index.html" ]] || echo "프론트엔드가 빌드되지 않았습니다. ./setup.sh 를 실행하세요." >&2
    mkdir -p "$root/data"
    # setsid: 터미널(세션)과 분리해 터미널을 닫아도 살아 있게 한다.
    setsid nohup "${uvicorn[@]}" >> "$logfile" 2>&1 < /dev/null &
    echo $! > "$pidfile"
    for _ in $(seq 60); do
        healthy && { echo "시작: $url  (로그: data/server.log)"; return 0; }
        kill -0 "$(cat "$pidfile")" 2> /dev/null || break
        sleep 0.25
    done
    echo "서버가 뜨지 않았습니다. 로그:" >&2
    tail -n 20 "$logfile" >&2
    exit 1
}

stop() {
    local pid
    if pid="$(running_pid)"; then
        kill "$pid"
        for _ in $(seq 40); do kill -0 "$pid" 2> /dev/null || break; sleep 0.25; done
        rm -f "$pidfile"
        echo "중지했습니다."
    elif healthy; then
        echo "포트 $port 에서 다른 방식(fg/dev)으로 띄운 서버가 돌고 있습니다. 그 터미널에서 끄세요." >&2
        exit 1
    else
        rm -f "$pidfile"
        echo "실행 중이 아닙니다."
    fi
}

case "${1:-open}" in
    open) start; open_url "$url" ;;
    start) start ;;
    stop) stop ;;
    restart) stop || true; start ;;
    status)
        if healthy; then echo "실행 중: $url (pid $(running_pid || echo '?'))"; else echo "꺼져 있습니다."; exit 1; fi ;;
    logs) touch "$logfile"; tail -n 50 -f "$logfile" ;;
    fg) exec "${uvicorn[@]}" ;;
    dev)
        export PATH="$root/.tools/node/bin:$PATH"
        "${uvicorn[@]}" --reload &
        trap 'kill $! 2>/dev/null' EXIT
        cd "$root/web/client"
        (sleep 2; open_url "http://127.0.0.1:5173/") &
        PS_PORT="$port" npx vite ;;
    -h|--help|help) sed -n '2,17p' "$0" ;;
    *) sed -n '2,17p' "$0" >&2; exit 1 ;;
esac

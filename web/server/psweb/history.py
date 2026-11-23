"""푼 문제 기록을 월별 마크다운 표로 쓴다: history/YYYY-MM.md (커밋 대상).

DB 의 AC 기록(solved_at)에서 매번 새로 만든다. 표는 날짜 → 번호 순.
"""
from collections import defaultdict

from . import db
from .config import ROOT

HISTORY = ROOT / "history"
TIERS = ["Bronze", "Silver", "Gold", "Platinum", "Diamond", "Ruby"]


def tier_name(level):
    if not level:
        return "Unrated"
    return f"{TIERS[(level - 1) // 5]} {['V', 'IV', 'III', 'II', 'I'][(level - 1) % 5]}"


def _cell(s):
    return str(s).replace("|", "\\|").replace("\n", " ")


def render_month(month, rows):
    lines = [
        f"# {month[:4]}년 {int(month[5:7])}월",
        "",
        f"{len(rows)}문제",
        "",
        "| 날짜 | 번호 | 문제 | 난이도 |",
        "|---|---|---|---|",
    ]
    for p in rows:
        lines.append(f"| {p['solved_at'][:10]} | [{p['id']}](https://www.acmicpc.net/problem/{p['id']}) "
                     f"| {_cell(p['title'])} | {tier_name(p['level'])} |")
    return "\n".join(lines) + "\n"


def build():
    """{저장소 기준 경로: 내용}. 지금까지 푼 모든 달."""
    months = defaultdict(list)
    for p in db.list_all():
        if p["status"] == "solved" and p["solved_at"]:
            months[p["solved_at"][:7]].append(p)
    out = {}
    for month, rows in months.items():
        rows.sort(key=lambda p: (p["solved_at"], p["id"]))
        out[f"history/{month}.md"] = render_month(month, rows)
    return out


def write():
    """history/ 를 다시 쓰고, 쓴 파일의 저장소 기준 경로 목록을 돌려준다."""
    files = build()
    HISTORY.mkdir(exist_ok=True)
    for rel, text in files.items():
        (ROOT / rel).write_text(text, encoding="utf-8")
    return files

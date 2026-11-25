"""BOJ 소스 페이지(/source/<제출 번호>)의 HTML 해석.

사용자가 브라우저에서 직접 연 페이지를 붙여넣거나 북마클릿으로 보낸 것만 다룬다. 서버는 BOJ 소스 페이지를 요청하지 않는다.

페이지 구조 (2026-11 기준):
    <textarea name="source" data-mime="text/x-c++src">코드</textarea>
    표 한 줄: 제출 번호 | 아이디 | 문제(/problem/<번호>) | 문제 제목 | 결과(span.result-ac 등) | 메모리 | 시간 | 언어 |
             코드 길이 | 제출한 시간(<a title="YYYY-MM-DD HH:MM:SS" data-timestamp="유닉스 초">2년 전</a>)
    BOJ 의 툴팁 스크립트가 돌고 난 DOM(북마클릿)에서는 title 이 data-original-title 로 옮겨져 있다.
"""
import datetime
import html
import re
from zoneinfo import ZoneInfo

KST = ZoneInfo("Asia/Seoul")

_MIME = {
    "text/x-c++src": "cpp", "text/x-csrc": "c", "text/x-python": "py", "text/x-java": "java",
    "text/x-kotlin": "kt", "text/x-rustsrc": "rs", "text/x-go": "go", "text/javascript": "js",
    "text/x-csharp": "cs", "text/x-ruby": "rb", "text/x-swift": "swift",
}


def lang_ext(name, mime=""):
    """BOJ 언어 이름("C++20", "PyPy3", "Java 11" …) → solution/ 확장자. 모르면 None."""
    n = name.strip()
    rules = [
        (r"^C\+\+", "cpp"), (r"^C#", "cs"), (r"^C(\d+|2x)?\b(?![+#])", "c"), (r"python|pypy", "py"),
        (r"^java(?!script)", "java"), (r"kotlin", "kt"), (r"^rust", "rs"), (r"^go\b", "go"),
        (r"node\.?js|javascript", "js"), (r"^ruby", "rb"), (r"^swift", "swift"),
    ]
    for pat, ext in rules:
        if re.search(pat, n, re.I):
            return ext
    return _MIME.get(mime)


def _text(fragment):
    return html.unescape(re.sub(r"<[^>]+>", "", fragment)).strip()


def _attr(tag, name):
    m = re.search(rf'\b{name}\s*=\s*(["\'])(.*?)\1', tag, re.S)
    return html.unescape(m.group(2)) if m else None


def parse(page):
    """BOJ 소스 페이지면 dict, 아니면 None."""
    m = re.search(r'(<textarea\b[^>]*\bname\s*=\s*["\']?source\b[^>]*>)(.*?)</textarea>', page, re.S | re.I)
    if not m:
        return None
    code = html.unescape(m.group(2))
    if code.startswith("\n"):            # HTML 규칙: textarea 의 첫 줄바꿈은 내용이 아니다
        code = code[1:]
    code = code.replace("\r\n", "\n").replace("\r", "\n").rstrip() + "\n"
    mime = _attr(m.group(1), "data-mime") or ""

    rows = re.findall(r"<tr\b[^>]*>(.*?)</tr>", page, re.S | re.I)
    row = next((r for r in rows if re.search(r'href=["\']/problem/\d+', r)), None)
    if row is None:
        return None
    cells = re.findall(r"<td\b[^>]*>(.*?)</td>", row, re.S | re.I)
    heads = [_text(h) for h in re.findall(r"<th\b[^>]*>(.*?)</th>", page, re.S | re.I)]

    def cell(label, fallback):
        i = heads.index(label) if label in heads else fallback
        return cells[i] if i < len(cells) else ""

    pid = int(re.search(r'href=["\']/problem/(\d+)', row).group(1))
    date = ""
    tm = re.search(r"<a\b[^>]*(?:data-timestamp|title|data-original-title)\s*=[^>]*>", row, re.S)
    if tm:
        t = _attr(tm.group(0), "title") or _attr(tm.group(0), "data-original-title") or ""
        if re.match(r"\d{4}-\d{2}-\d{2}", t):
            date = t[:10]
        elif (ts := _attr(tm.group(0), "data-timestamp")) and ts.isdigit():
            date = datetime.datetime.fromtimestamp(int(ts), KST).date().isoformat()
    language = _text(cell("언어", 7))
    sid = _text(cell("제출 번호", 0))
    result_cell = cell("결과", 4)
    return {
        "submissionId": int(sid) if sid.isdigit() else None,
        "user": _text(cell("아이디", 1)),
        "problemId": pid,
        "problemTitle": _text(cell("문제 제목", 3)),
        "result": _text(result_cell),
        "accepted": "result-ac" in result_cell,
        "language": language,
        "ext": lang_ext(language, mime),
        "date": date,
        "code": code,
    }

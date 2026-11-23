"""BOJ 문제 페이지를 가져와 문제 작업 폴더에 저장한다.

problems/<번호>/
    statement.json          웹 페이지용: 제목, 제한, 섹션별 HTML(정리됨), 예제
    problem.md              VSCode 에서 읽는 지문 (티어/태그는 넣지 않는다)
    inputs/sample{k}.in     예제 입력
    outputs/sample{k}.out   예제 출력

BOJ 는 공식 API 가 없고, /problem/<번호> 는 robots.txt 에서 허용된 경로다. 요청은 문제당 1회.
"""
import html
import json
import re
import time
import urllib.error
import urllib.request
from html.parser import HTMLParser

from .config import PROBLEMS

UA = "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/130.0 Safari/537.36"
SECTIONS = [("description", "문제"), ("input", "입력"), ("output", "출력"), ("limit", "제한"), ("hint", "힌트")]
VOID = {"br", "img", "hr", "meta", "link", "input", "col", "area", "base", "source", "wbr"}
DROP = {"script", "style", "iframe", "object", "embed", "form", "button"}
_last_request = [0.0]


class BojError(Exception):
    pass


class Capture(HTMLParser):
    """지정한 id 를 가진 요소의 안쪽 HTML(정리된 것)과 텍스트를 모은다."""

    def __init__(self, want):
        super().__init__(convert_charrefs=False)
        self.want = want
        self.html, self.text = {}, {}
        self.stack = []        # (tag, 수집 id 또는 None)
        self.active = []
        self.drop_depth = 0

    def _emit(self, raw, txt=""):
        for k in self.active:
            self.html[k].append(raw)
            self.text[k].append(txt)

    def handle_starttag(self, tag, attrs):
        a = dict(attrs)
        if self.active and (self.drop_depth or tag in DROP):
            if tag not in VOID:
                self.drop_depth += 1
                self.stack.append((tag, None))
            return
        if self.active:
            keep = []
            for k, v in attrs:
                if k.startswith("on") or v is None:
                    continue
                if k in ("href", "src"):
                    if v.strip().lower().startswith("javascript:"):
                        continue
                    if v.startswith("/"):
                        v = "https://www.acmicpc.net" + v
                if k in ("href", "src", "alt", "title", "class", "colspan", "rowspan", "width", "height", "style"):
                    keep.append(f' {k}="{html.escape(v, quote=True)}"')
            extra = ' target="_blank" rel="noreferrer"' if tag == "a" else ""
            txt = {"br": "\n", "li": "\n- ", "p": "\n", "pre": "\n```\n", "tr": "\n", "td": " | ", "th": " | ",
                   "img": f"[image: {a.get('src', '')}]", "sup": "^", "sub": "_"}.get(tag, "")
            self._emit(f"<{tag}{''.join(keep)}{extra}>", txt)
        if tag in VOID:
            return
        cap = a.get("id")
        cap = cap if cap and (cap in self.want or re.fullmatch(r"sample-(input|output)-\d+", cap)) else None
        self.stack.append((tag, cap))
        if cap:
            self.html[cap], self.text[cap] = [], []
            self.active.append(cap)

    def handle_startendtag(self, tag, attrs):
        self.handle_starttag(tag, attrs)

    def handle_endtag(self, tag):
        if tag in VOID:
            return
        while self.stack:
            t, cap = self.stack.pop()
            if self.drop_depth:
                self.drop_depth -= 1
                if t == tag:
                    return
                continue
            if cap:
                self.active.remove(cap)
            elif self.active:
                self._emit(f"</{t}>", {"p": "\n", "pre": "\n```\n", "li": "", "div": "\n"}.get(t, ""))
            if t == tag:
                break

    def handle_data(self, data):
        if self.active and not self.drop_depth:
            self._emit(html.escape(data, quote=False), data)

    def handle_entityref(self, name):
        self.handle_data(html.unescape(f"&{name};"))

    def handle_charref(self, name):
        self.handle_data(html.unescape(f"&#{name};"))


def clean_text(s):
    s = s.replace("\xa0", " ").replace("\r", "")
    out, code = [], False
    for ln in s.split("\n"):
        if ln.strip() == "```":
            code = not code
            out.append("```")
        else:
            out.append(ln.rstrip() if code else ln.strip())
    s = "\n".join(out)
    s = re.sub(r"```\n+", "```\n", s)
    s = re.sub(r"\n+```(?=\n|$)", "\n```", s)
    return re.sub(r"\n{3,}", "\n\n", s).strip()


def download(pid):
    wait = 1.0 - (time.monotonic() - _last_request[0])     # BOJ 에 연속 요청을 보내지 않는다
    if wait > 0:
        time.sleep(wait)
    _last_request[0] = time.monotonic()
    req = urllib.request.Request(f"https://www.acmicpc.net/problem/{pid}",
                                 headers={"User-Agent": UA, "Accept-Language": "ko-KR,ko;q=0.9"})
    try:
        with urllib.request.urlopen(req, timeout=20) as r:
            return r.read().decode("utf-8", errors="replace")
    except urllib.error.HTTPError as e:
        raise BojError(f"BOJ HTTP {e.code}: 문제 {pid}") from None


def parse(doc):
    want = {"problem_title", "problem-info"} | {f"problem_{k}" for k, _ in SECTIONS}
    c = Capture(want)
    c.feed(doc)
    H = {k: "".join(v).strip() for k, v in c.html.items()}
    T = {k: "".join(v) for k, v in c.text.items()}
    title = clean_text(T.get("problem_title", ""))
    if not title:
        raise BojError("문제 페이지에서 제목을 찾지 못했습니다.")

    rows = [r for r in (clean_text(x) for x in T.get("problem-info", "").split("\n")) if "|" in r]
    info = {}
    if len(rows) >= 2:
        heads = [c.strip() for c in rows[0].split("|") if c.strip()]
        vals = [c.strip() for c in rows[1].split("|") if c.strip()]
        info = dict(zip(heads, vals))

    samples = []
    k = 1
    while f"sample-input-{k}" in T:
        norm = lambda s: "\n".join(ln.rstrip() for ln in s.replace("\r", "").split("\n")).strip("\n") + "\n"  # noqa: E731
        samples.append({"input": norm(T[f"sample-input-{k}"]), "output": norm(T.get(f"sample-output-{k}", ""))})
        k += 1

    return {
        "title": title,
        "timeLimit": info.get("시간 제한", ""),
        "memoryLimit": info.get("메모리 제한", ""),
        "sections": [{"key": key, "name": name, "html": H.get(f"problem_{key}", ""),
                      "text": clean_text(T.get(f"problem_{key}", ""))}
                     for key, name in SECTIONS if clean_text(T.get(f"problem_{key}", ""))],
        "samples": samples,
    }


def time_limit_ms(st):
    m = re.search(r"[0-9.]+", st.get("timeLimit", ""))
    return int(float(m.group()) * 1000) if m else 2000


def save(pid, st):
    d = PROBLEMS / str(pid)
    for sub in ("inputs", "outputs", "answers"):
        (d / sub).mkdir(parents=True, exist_ok=True)
    (d / "statement.json").write_text(json.dumps(st, ensure_ascii=False), encoding="utf-8")

    md = [f"# {pid}. {st['title']}", "", f"https://www.acmicpc.net/problem/{pid}", ""]
    if st["timeLimit"]:
        md.append(f"- 시간 제한: {st['timeLimit']}")
    if st["memoryLimit"]:
        md.append(f"- 메모리 제한: {st['memoryLimit']}")
    md.append("")
    for s in st["sections"]:
        md += [f"## {s['name']}", "", s["text"], ""]
    for i, s in enumerate(st["samples"]):
        md += [f"## 예제 {i}", "", "```", s["input"].rstrip("\n"), "```", "", "```", s["output"].rstrip("\n"), "```", ""]
    (d / "problem.md").write_text("\n".join(md), encoding="utf-8")

    for old in list((d / "inputs").glob("sample*.in")) + list((d / "outputs").glob("sample*.out")):
        old.unlink()
    for i, s in enumerate(st["samples"]):
        (d / "inputs" / f"sample{i}.in").write_text(s["input"], encoding="utf-8")
        (d / "outputs" / f"sample{i}.out").write_text(s["output"], encoding="utf-8")


def load(pid):
    path = PROBLEMS / str(pid) / "statement.json"
    return json.loads(path.read_text(encoding="utf-8")) if path.exists() else None


def fetch(pid, refresh=False):
    """캐시가 있으면 그대로, 없으면 받아서 저장. statement 를 돌려준다."""
    st = None if refresh else load(pid)
    if st is None:
        st = parse(download(pid))
        save(pid, st)
    return st

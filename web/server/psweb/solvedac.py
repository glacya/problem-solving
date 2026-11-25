"""solved.ac API v4 클라이언트.

모든 요청에 x-client-id / x-client-secret 이 필요하다 (https://backstage.solved.ac/ko/developer/apps).
Python 기본 User-Agent 는 Cloudflare 가 막으므로 UA 를 붙인다.
"""
import json
import urllib.error
import urllib.parse
import urllib.request

from .config import env

UA = "glacya-ps/1.0 (+https://github.com/glacya/problem-solving)"


class ApiError(Exception):
    def __init__(self, status, code, message):
        super().__init__(f"solved.ac {status} {code}: {message}")
        self.status, self.code, self.message = status, code, message


def get(path, **params):
    cid, secret = env("SOLVEDAC_CLIENT_ID"), env("SOLVEDAC_CLIENT_SECRET")
    if not cid or not secret:
        raise ApiError(0, "NO_KEY", ".env 에 SOLVEDAC_CLIENT_ID / SOLVEDAC_CLIENT_SECRET 을 설정하세요.")
    url = f"{env('SOLVEDAC_API', 'https://solved.ac')}{path}"
    if params:
        url += "?" + urllib.parse.urlencode(params)
    req = urllib.request.Request(url, headers={
        "User-Agent": UA, "Accept": "application/json", "x-client-id": cid, "x-client-secret": secret})
    try:
        with urllib.request.urlopen(req, timeout=20) as r:
            return json.load(r)
    except urllib.error.HTTPError as e:
        body = e.read().decode(errors="replace")
        try:
            err = json.loads(body).get("error", {})
        except ValueError:
            err = {}
        raise ApiError(e.code, err.get("code", "?"), err.get("message", body[:200])) from None


def boj_id(p):
    """BOJ 문제 번호. BOJ 문제가 아니면 None.

    solved.ac 의 problemId 는 BOJ 외의 저지 문제에도 붙는 자체 번호다 (예: DOJ 434 → 100434).
    """
    for j in p.get("judgeIds", []):
        if j.get("ojId") == "boj" and not j.get("subId"):
            return int(j["problemId"])
    return None


def meta_of(p):
    """API 의 문제 객체에서 필요한 것만. BOJ 문제가 아니면 None."""
    pid = boj_id(p)
    if pid is None:
        return None
    titles = p.get("titles", [])
    title = next((t["title"] for t in titles if t.get("language") == "ko"),
                 next((t["title"] for t in titles if t.get("isOriginal")), titles[0]["title"] if titles else ""))
    return {
        "id": pid,
        "title": title,
        "level": p.get("level", 0),
        "tags": [t["key"] for t in p.get("tags", []) if not t.get("isMeta")],
        "solved_count": p.get("solvedUserCount"),
    }


def check_query(query):
    """쿼리 진단. 오류(예: 없는 핸들 → 이미 푼 문제가 섞임)가 있으면 ApiError."""
    diags = get("/api/v4/search/analyze", q=query).get("diagnostics", [])
    errors = [d for d in diags if d["severity"] == "error"]
    if errors:
        raise ApiError(400, "BAD_QUERY", "; ".join(f"{query[d['start']:d['end']]!r}: {d['message']}" for d in errors))
    return [d["message"] for d in diags]


def random_problems(query, n):
    check_query(query)
    picked = {}
    for _ in range(n * 3):            # 같은 문제가 다시 나올 수 있으니 여유 있게
        if len(picked) == n:
            break
        try:
            m = meta_of(get("/api/v4/search/random-problem", q=query))
        except ApiError as e:
            if e.status == 404:
                break
            raise
        if m is not None:              # oj:boj 를 뺀 쿼리라도 다른 저지 문제는 버린다
            picked.setdefault(m["id"], m)
    return list(picked.values())


def problem(pid):
    try:
        return meta_of(get(f"/api/v4/problems/{int(pid)}"))
    except ApiError as e:
        if e.status == 404:
            return None
        raise


def solved_problems(handle):
    """그 핸들이 푼 BOJ 문제 전부 (메타 목록, 번호순). 50개씩 나눠 받는다 (페이지당 3 unit).

    1000문제면 20번 요청하므로 몇 초 걸린다. 첫 페이지로 전체 수를 알고 나머지는 몇 개씩 동시에 받는다.
    """
    from concurrent.futures import ThreadPoolExecutor

    q = f"@{handle} oj:boj"

    def page(n):
        return get("/api/v4/problems", q=q, page=n, sort="id", direction="asc")

    first = page(1)
    size = first.get("pageSize") or 50
    pages = -(-first.get("total", 0) // size)
    results = [first]
    if pages > 1:
        with ThreadPoolExecutor(4) as ex:
            results += list(ex.map(page, range(2, pages + 1)))
    out = {}
    for r in results:
        for p in r.get("problems", []):
            m = meta_of(p)
            if m is not None:
                out[m["id"]] = m
    return [out[k] for k in sorted(out)]


def solved_by(pid, handle):
    """그 핸들이 문제를 푼 적이 있는지 (특정 제출의 결과가 아니다)."""
    return get("/api/v4/problems", q=f"id:{pid} @{handle}").get("total", 0) > 0

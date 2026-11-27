"""FastAPI 앱: /api/* 와 빌드된 프론트엔드(web/client/dist).

    .venv/bin/python -m uvicorn psweb.app:app --app-dir web/server --port 8710

127.0.0.1 에서만 쓰는 개인용 서버다. 다른 사이트가 이 서버로 요청을 보내지 못하도록
Host / Origin 을 확인하고, 상태를 바꾸는 요청은 JSON 만 받는다.
"""
import logging
from contextlib import asynccontextmanager

from fastapi import FastAPI, HTTPException, Request
from fastapi.responses import FileResponse, JSONResponse, Response
from pydantic import BaseModel

from . import backfill, boj, cleanup, gitops, bundle, db, judge, solvedac, watcher, workspace
from .config import CLIENT_DIST, LANGS, SOURCE_LANGS, default_query, env, normalize_query

logging.basicConfig(level=logging.INFO, format="%(asctime)s %(name)s %(levelname)s %(message)s")


@asynccontextmanager
async def lifespan(_app):
    moved = gitops.migrate_layout()     # solution/ 폴더 구조가 바뀌었으면 옮긴다
    if moved:
        logging.getLogger("psweb").info("solution 파일 %d개를 새 폴더 구조로 옮겼습니다.", len(moved))
    watcher.start()        # 서버가 꺼져 있던 동안 남은 AC 확인도 이어서 한다
    yield
    watcher.stop()


app = FastAPI(title="ps", lifespan=lifespan)

LOCAL_HOSTS = ("127.0.0.1", "localhost")

# BOJ 페이지에서 누르는 북마클릿만 부를 수 있는 경로 (다른 사이트는 여전히 막는다).
BOJ_ORIGINS = ("https://www.acmicpc.net", "https://acmicpc.net")
BOJ_PATHS = ("/api/backfill/page", "/api/backfill/next")


def _cors(resp, origin):
    resp.headers["Access-Control-Allow-Origin"] = origin
    resp.headers["Vary"] = "Origin"
    return resp


@app.middleware("http")
async def local_only(request: Request, call_next):
    host = (request.headers.get("host") or "").rsplit(":", 1)[0]
    origin = request.headers.get("origin")
    if host not in LOCAL_HOSTS:
        return JSONResponse({"detail": "local only"}, status_code=403)          # DNS rebinding 방지
    if origin in BOJ_ORIGINS and request.url.path in BOJ_PATHS:
        if request.method == "OPTIONS":       # CORS 사전 요청 (+ Chrome 의 로컬 네트워크 접근)
            resp = Response(status_code=204)
            resp.headers["Access-Control-Allow-Methods"] = "GET, POST"
            resp.headers["Access-Control-Allow-Headers"] = "Content-Type"
            resp.headers["Access-Control-Allow-Private-Network"] = "true"
            resp.headers["Access-Control-Max-Age"] = "600"
            return _cors(resp, origin)
        if request.method == "POST" and not (request.headers.get("content-type") or "").startswith("application/json"):
            return _cors(JSONResponse({"detail": "json only"}, status_code=415), origin)
        return _cors(await call_next(request), origin)
    if request.method not in ("GET", "HEAD"):
        if origin and origin.split("://", 1)[-1].rsplit(":", 1)[0] not in LOCAL_HOSTS:
            return JSONResponse({"detail": "cross-origin"}, status_code=403)
        if not (request.headers.get("content-type") or "").startswith("application/json"):
            return JSONResponse({"detail": "json only"}, status_code=415)
    return await call_next(request)


# ---- 직렬화: 안 푼 문제의 티어/태그는 내보내지 않는다 ----

def public(p):
    if p is None:
        return None
    solved = p["status"] == "solved"
    out = {k: p[k] for k in ("id", "title", "status", "reason", "source", "language", "time_limit", "memory_limit",
                             "added_at", "updated_at", "started_at", "submitted_at", "solved_at", "commit_log")}
    out["level"] = p["level"] if solved else None
    out["tags"] = p["tags"] if solved else None
    out["watching"] = bool(p["watch_until"] and p["watch_until"] > db.now() and not solved)
    out["watch_until"] = p["watch_until"] if out["watching"] else None
    out["memo"] = workspace.memo_excerpt(p["id"])
    return out


def need(pid):
    p = db.get(pid)
    if p is None:
        raise HTTPException(404, f"{pid} 는 기록에 없습니다.")
    return p


def need_lang(lang):
    if lang not in LANGS:
        raise HTTPException(400, f"lang 은 {', '.join(LANGS)} 중 하나")
    return lang


def add_problem(pid, meta=None, source=""):
    """BOJ 에서 지문을 받고(캐시 우선) 기록에 넣는다. solved.ac 메타가 없으면 따로 받는다."""
    st = boj.fetch(pid)
    if meta is None:
        try:
            meta = solvedac.problem(pid) or {}
        except solvedac.ApiError:
            meta = {}
    fields = {"title": st["title"], "time_limit": st["timeLimit"], "memory_limit": st["memoryLimit"]}
    for k in ("level", "tags", "solved_count"):
        if meta.get(k) is not None:
            fields[k] = meta[k]
    if db.get(pid) is None:
        fields["source"] = source
    return db.upsert(pid, **fields)


def api_error(fn):
    """외부 서비스 오류를 4xx/5xx 로."""
    import functools

    @functools.wraps(fn)
    def wrap(*a, **kw):
        try:
            return fn(*a, **kw)
        except solvedac.ApiError as e:
            raise HTTPException(400 if e.status in (0, 400) else 502, str(e)) from None
        except boj.BojError as e:
            raise HTTPException(502, str(e)) from None
        except bundle.BundleError as e:
            raise HTTPException(400, str(e)) from None
        except backfill.BackfillError as e:
            raise HTTPException(e.status, str(e)) from None
    return wrap


# ---- 설정 ----

@app.get("/api/health")
def health():
    return {"ok": True}


@app.get("/api/settings")
def settings():
    return {
        "handle": env("BOJ_HANDLE"),
        "hasKey": bool(env("SOLVEDAC_CLIENT_ID") and env("SOLVEDAC_CLIENT_SECRET")),
        "defaultQuery": default_query(),
        "langs": [{"id": k, "label": v["label"], "boj": v["boj"]} for k, v in LANGS.items()],
        "sourceLangs": [{"id": k, "label": v[0]} for k, v in SOURCE_LANGS.items()],
    }


# ---- 문제 목록 / 추가 ----

@app.get("/api/problems")
def problems():
    return [public(p) for p in db.list_all()]


class Draw(BaseModel):
    n: int = 1
    query: str | None = None


@app.post("/api/draw")
@api_error
def draw(body: Draw):
    if not 1 <= body.n <= 20:
        raise HTTPException(400, "n 은 1~20")
    query = normalize_query(body.query) if (body.query or "").strip() else default_query()
    picked = solvedac.random_problems(query, body.n)
    if not picked:
        raise HTTPException(404, f"'{query}' 에 맞는 문제가 없습니다.")
    added, failed = [], []
    for m in picked:                  # 한 문제가 실패해도 나머지는 넣는다
        try:
            added.append(public(add_problem(m["id"], m, source=f"rand: {query}")))
        except boj.BojError as e:
            failed.append({"id": m["id"], "error": str(e)})
    if not added:
        raise HTTPException(502, "; ".join(f["error"] for f in failed))
    return {"query": query, "added": added, "failed": failed}


class AddById(BaseModel):
    id: int


@app.post("/api/problems")
@api_error
def add(body: AddById):
    return public(add_problem(body.id, source="manual"))


# ---- 문제 하나 ----

@app.get("/api/problems/{pid}")
def detail(pid: int):
    p = need(pid)
    st = boj.load(pid)
    lang = p["language"]
    return {
        "problem": public(p),
        "statement": st,
        "tests": workspace.tests(pid),
        "langs": workspace.existing_langs(pid),
        "dir": str(workspace.pdir(pid)),
        "editorUri": workspace.editor_uri(workspace.source(pid, lang)),
    }


class Status(BaseModel):
    status: str
    reason: str | None = None


@app.post("/api/problems/{pid}/status")
def set_status(pid: int, body: Status):
    need(pid)
    try:
        return public(db.set_status(pid, body.status, body.reason))
    except ValueError as e:
        raise HTTPException(400, str(e)) from None


@app.delete("/api/problems/{pid}")
def remove(pid: int):
    need(pid)
    db.delete(pid)
    return {"ok": True}


class Lang(BaseModel):
    lang: str = "cpp"


class Memo(BaseModel):
    text: str


@app.get("/api/problems/{pid}/memo")
def get_memo(pid: int):
    need(pid)
    text, updated = workspace.read_memo(pid)
    return {"text": text, "updatedAt": updated, "path": str(workspace.memo_path(pid))}


@app.put("/api/problems/{pid}/memo")
def put_memo(pid: int, body: Memo):
    need(pid)
    if len(body.text) > 200_000:
        raise HTTPException(413, "메모가 너무 깁니다.")
    text, updated = workspace.write_memo(pid, body.text)
    return {"text": text, "updatedAt": updated, "path": str(workspace.memo_path(pid))}


@app.post("/api/problems/{pid}/start")
def start(pid: int, body: Lang):
    """현재 문제로 정하고, 풀이 파일을 (없으면 템플릿으로) 만들어 VSCode 로 연다."""
    need(pid)
    lang = need_lang(body.lang)
    if boj.load(pid) is None:
        boj.fetch(pid)
    path = workspace.ensure_source(pid, lang)
    fields = {"language": lang}
    if need(pid)["presolved"] is None and env("BOJ_HANDLE"):
        try:                    # 예전에 푼 문제면 제출본 없이 하는 자동 AC 확인에서 뺀다 (watcher.recently_edited)
            fields["presolved"] = int(solvedac.solved_by(pid, env("BOJ_HANDLE")))
        except solvedac.ApiError:
            pass
    db.upsert(pid, **fields)
    p = db.set_status(pid, "current")
    return {"problem": public(p), "path": str(path), "opened": workspace.open_in_editor(path),
            "editorUri": workspace.editor_uri(path)}


@app.post("/api/problems/{pid}/open-editor")
def open_editor(pid: int, body: Lang):
    need(pid)
    path = workspace.ensure_source(pid, need_lang(body.lang))
    return {"path": str(path), "opened": workspace.open_in_editor(path), "editorUri": workspace.editor_uri(path)}


# ---- 제출 준비 ----

def _bundle(pid, lang):
    code, used = bundle.bundle(workspace.source(pid, lang), lang)
    return {"lang": lang, "code": code, "used": used, "lines": code.count("\n"), "bytes": len(code.encode())}


@app.get("/api/problems/{pid}/bundle")
@api_error
def get_bundle(pid: int, lang: str = "cpp"):
    need(pid)
    return _bundle(pid, need_lang(lang))


@app.post("/api/problems/{pid}/judge")
@api_error
def run_judge(pid: int, body: Lang):
    need(pid)
    lang = need_lang(body.lang)
    b = _bundle(pid, lang)
    st = boj.load(pid) or {}
    res = judge.judge(workspace.pdir(pid), b["code"], lang, boj.time_limit_ms(st))
    res["bundle"] = {k: b[k] for k in ("lines", "bytes", "used")}
    return res


@app.post("/api/problems/{pid}/submit")
@api_error
def submit(pid: int, body: Lang):
    """지금의 번들을 제출본으로 떠 둔다. AC 가 확인되면 이 파일이 커밋된다."""
    need(pid)
    lang = need_lang(body.lang)
    b = _bundle(pid, lang)
    (workspace.pdir(pid) / f".submitted.{lang}").write_text(b["code"], encoding="utf-8")
    # 이제부터 서버가 AC 를 확인한다 (브라우저를 닫거나 서버를 재시작해도 DB 에 남아 이어서 본다).
    db.upsert(pid, language=lang, submitted_at=db.now(), watch_until=db.now(watcher.WATCH_MINUTES))
    return {**b, "submitUrl": f"https://www.acmicpc.net/submit/{pid}"}


@app.post("/api/problems/{pid}/check")
@api_error
def check(pid: int):
    """지금 바로 AC 를 확인한다 (서버도 제출 후 30초마다 확인한다)."""
    need(pid)
    res = watcher.confirm_ac(pid)
    return {**res, "problem": public(db.get(pid))}


@app.post("/api/problems/{pid}/mark-solved")
@api_error
def mark_solved(pid: int, body: Lang):
    """BOJ 에서 AC 를 직접 확인했을 때: solved.ac 확인 없이 AC 로 기록하고 커밋한다.

    BOJ 를 서버가 직접 확인할 수는 없다 (채점 현황은 robots.txt 에서 막혀 있고, 사용자 페이지는 로그인이 필요하다).
    제출본이 없으면 지금 코드의 번들을 커밋한다.
    """
    p = need(pid)
    lang = need_lang(body.lang)
    if not (workspace.pdir(pid) / f".submitted.{p['language']}").exists():
        db.upsert(pid, language=lang)        # 제출본이 없으면 화면에서 고른 언어의 지금 코드를 번들해 커밋한다
    res = watcher.confirm_ac(pid, manual=True)
    return {**res, "problem": public(db.get(pid))}


@app.post("/api/problems/{pid}/watch")
def watch(pid: int):
    """AC 확인을 (다시) 시작한다."""
    need(pid)
    return public(db.upsert(pid, watch_until=db.now(watcher.WATCH_MINUTES)))


@app.delete("/api/problems/{pid}/watch")
def unwatch(pid: int):
    need(pid)
    return public(db.upsert(pid, watch_until=None))


# ---- 예전 소스 모으기 ----

@app.get("/api/backfill/missing")
@api_error
def backfill_missing(refresh: bool = False):
    """푼 문제 중 solution/ 에 소스가 없는 것. solved.ac 목록은 캐시를 쓰고 refresh 때만 다시 받는다 (몇 초 걸린다)."""
    return backfill.missing(refresh)


@app.get("/api/backfill/next")
@api_error
def backfill_next(after: int = 0):
    """북마클릿: 다음으로 소스를 넣을 문제와 그 문제의 내 맞은 제출 (BOJ 채점 현황 주소)."""
    return {"next": backfill.next_missing(after)}


class Page(BaseModel):
    html: str
    confirm: bool = False


@app.post("/api/backfill/parse")
@api_error
def backfill_parse(body: Page):
    """붙여넣은 BOJ 소스 페이지 해석 (저장하지 않는다)."""
    return backfill.parse_page(body.html)


@app.post("/api/backfill/page")
@api_error
def backfill_page(body: Page):
    """북마클릿: BOJ 소스 페이지를 받아 바로 저장하고 다음 문제를 알려 준다."""
    if len(body.html) > 3_000_000:
        raise HTTPException(413, "페이지가 너무 큽니다.")
    return backfill.from_page(body.html, body.confirm)


@app.get("/api/backfill/{pid}")
@api_error
def backfill_lookup(pid: int):
    return backfill.lookup(pid)


class Backfill(BaseModel):
    date: str
    lang: str
    code: str


@app.post("/api/backfill/{pid}")
@api_error
def backfill_add(pid: int, body: Backfill):
    return backfill.add(pid, body.date, body.lang, body.code)


# ---- 풀이 폴더 정리 ----

@app.get("/api/cleanup")
def cleanup_preview():
    """지금 정리하면 지워질 풀이 폴더 (AC 를 받고 마지막 제출일로부터 3일 이상 지난 것)."""
    return {"keepDays": cleanup.KEEP_DAYS, "candidates": cleanup.candidates()}


@app.post("/api/cleanup")
def cleanup_run():
    with watcher.commit_lock:
        return {"removed": cleanup.run()}


# ---- 프론트엔드 ----

@app.get("/{path:path}", include_in_schema=False)
def spa(path: str):
    if path.startswith("api/"):
        raise HTTPException(404)
    f = (CLIENT_DIST / path).resolve()
    if path and f.is_file() and CLIENT_DIST.resolve() in f.parents:
        return FileResponse(f)
    index = CLIENT_DIST / "index.html"
    if not index.exists():
        return JSONResponse({"detail": "프론트엔드가 빌드되지 않았습니다. ./setup.sh 를 실행하세요."}, status_code=503)
    return FileResponse(index)

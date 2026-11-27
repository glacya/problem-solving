"""문제 기록 저장소 (SQLite, data/ps.db).

상태:
    queued   대기
    current  풀이 중 (항상 하나뿐)
    paused   풀다 만 — 다른 문제를 시작해서 자동으로 멈춘 것
    later    나중에 풀기 — 직접 미룬 것 (사유를 적을 수 있다)
    skipped  스킵 (사유)
    solved   AC

서버 상태는 전부 여기(와 problems/ 의 파일)에 있으므로, 서버를 재시작해도 이어서 동작한다.
"""
import datetime
import json
import threading

try:
    import sqlite3
except ModuleNotFoundError:        # SQLite 없이 빌드된 Python (pyenv 에서 libsqlite3-dev 가 없던 경우)
    from pysqlite3 import dbapi2 as sqlite3

from .config import DATA

STATUSES = ("queued", "current", "paused", "later", "skipped", "solved")

_lock = threading.Lock()
_SCHEMA = """
CREATE TABLE IF NOT EXISTS problems (
    id            INTEGER PRIMARY KEY,
    title         TEXT NOT NULL DEFAULT '',
    level         INTEGER NOT NULL DEFAULT 0,
    tags          TEXT NOT NULL DEFAULT '[]',
    solved_count  INTEGER,
    time_limit    TEXT NOT NULL DEFAULT '',
    memory_limit  TEXT NOT NULL DEFAULT '',
    status        TEXT NOT NULL DEFAULT 'queued',
    reason        TEXT NOT NULL DEFAULT '',
    source        TEXT NOT NULL DEFAULT '',
    language      TEXT NOT NULL DEFAULT 'cpp',
    added_at      TEXT NOT NULL,
    updated_at    TEXT NOT NULL,
    started_at    TEXT,
    submitted_at  TEXT,
    solved_at     TEXT              -- 푼 날짜 YYYY-MM-DD (시각은 저장하지 않는다)
);
"""

# 나중에 추가한 열. 기존 DB 에는 ALTER TABLE 로 붙인다 (열을 지우거나 바꾸지 않는다).
_COLUMNS = {
    "watch_until": "TEXT",          # 이 시각까지 서버가 AC 를 확인한다 (제출 후)
    "commit_log": "TEXT NOT NULL DEFAULT ''",
    "presolved": "INTEGER",         # 풀기 시작할 때 solved.ac 에 이미 AC 였는지 (1/0, 모르면 NULL)
}
_migrated = False


def today():
    return datetime.date.today().isoformat()


def now(delta_minutes=0):
    t = datetime.datetime.now() + datetime.timedelta(minutes=delta_minutes)
    return t.isoformat(timespec="seconds")


def connect():
    global _migrated
    DATA.mkdir(exist_ok=True)
    con = sqlite3.connect(DATA / "ps.db", timeout=10)
    con.row_factory = sqlite3.Row
    if not _migrated:
        con.executescript(_SCHEMA)
        have = {r["name"] for r in con.execute("PRAGMA table_info(problems)")}
        for col, decl in _COLUMNS.items():
            if col not in have:
                con.execute(f"ALTER TABLE problems ADD COLUMN {col} {decl}")
        # 예전에는 푼 시각까지 저장했다. 푼 기록은 날짜만 쓴다.
        con.execute("UPDATE problems SET solved_at = substr(solved_at, 1, 10) WHERE length(solved_at) > 10")
        con.commit()
        _migrated = True
    return con


def _row(r):
    if r is None:
        return None
    d = dict(r)
    d["tags"] = json.loads(d["tags"])
    return d


def get(pid):
    with connect() as con:
        return _row(con.execute("SELECT * FROM problems WHERE id = ?", (pid,)).fetchone())


def list_all():
    with connect() as con:
        rows = con.execute("SELECT * FROM problems ORDER BY updated_at DESC").fetchall()
    return [_row(r) for r in rows]


def watching():
    """서버가 AC 를 확인해야 하는 문제들."""
    with connect() as con:
        rows = con.execute("SELECT * FROM problems WHERE watch_until IS NOT NULL AND watch_until > ? "
                           "AND status != 'solved'", (now(),)).fetchall()
    return [_row(r) for r in rows]


def upsert(pid, **fields):
    """없으면 만들고, 있으면 주어진 필드만 고친다."""
    if "tags" in fields:
        fields["tags"] = json.dumps(fields["tags"], ensure_ascii=False)
    with _lock, connect() as con:
        if con.execute("SELECT 1 FROM problems WHERE id = ?", (pid,)).fetchone() is None:
            con.execute("INSERT INTO problems (id, added_at, updated_at) VALUES (?, ?, ?)", (pid, now(), now()))
        if fields:
            cols = ", ".join(f"{k} = ?" for k in fields)
            con.execute(f"UPDATE problems SET {cols}, updated_at = ? WHERE id = ?", (*fields.values(), now(), pid))
    return get(pid)


def set_status(pid, status, reason=None):
    if status not in STATUSES:
        raise ValueError(f"status 는 {', '.join(STATUSES)} 중 하나")
    with _lock, connect() as con:
        if status == "current":
            # 풀던 문제는 '풀다 만' 으로 돌린다.
            con.execute("UPDATE problems SET status = 'paused', updated_at = ? WHERE status = 'current' AND id != ?",
                        (now(), pid))
            con.execute("UPDATE problems SET started_at = COALESCE(started_at, ?) WHERE id = ?", (now(), pid))
        if status == "solved":
            con.execute("UPDATE problems SET solved_at = ?, watch_until = NULL WHERE id = ?", (today(), pid))
        if reason is None and status in ("queued", "current", "paused", "solved"):
            reason = ""
        if reason is None:
            con.execute("UPDATE problems SET status = ?, updated_at = ? WHERE id = ?", (status, now(), pid))
        else:
            con.execute("UPDATE problems SET status = ?, reason = ?, updated_at = ? WHERE id = ?",
                        (status, reason, now(), pid))
    return get(pid)


def delete(pid):
    with _lock, connect() as con:
        con.execute("DELETE FROM problems WHERE id = ?", (pid,))

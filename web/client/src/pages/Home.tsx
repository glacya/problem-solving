import { useState } from "react";
import { Link, useNavigate } from "react-router";
import { api, openEditorResult, useProblemMutation, useProblems, useSettings, type Lang, type Problem } from "../api";
import { ErrorBox, LaterButton, ProblemLink, SkipButton, StatusBadge, Tags, Tier, formatDate } from "../components";

export default function Home() {
  const problems = useProblems();
  const list = problems.data ?? [];
  const current = list.find((p) => p.status === "current");
  const paused = list.filter((p) => p.status === "paused");
  const later = list.filter((p) => p.status === "later");
  const queued = list.filter((p) => p.status === "queued");
  const watching = list.filter((p) => p.watching && p.status !== "current");
  const solved = list.filter((p) => p.status === "solved").sort((a, b) => (b.solved_at ?? "").localeCompare(a.solved_at ?? ""));

  return (
    <div className="stack">
      <CurrentCard p={current} />
      {watching.length > 0 && (
        <p className="small muted">
          AC 확인 중: {watching.map((p) => <ProblemLink key={p.id} p={p} />)}
        </p>
      )}
      <div className="grid-2">
        <ProblemList title="풀다 만 문제" empty="다른 문제로 넘어가면 풀던 문제가 여기에 남습니다." items={paused} startLabel="이어서 풀기" />
        <ProblemList title="나중에 풀기" empty="미뤄 둔 문제가 없습니다." items={later} startLabel="이어서 풀기" />
      </div>
      <ProblemList title="대기 중" empty="대기 중인 문제가 없습니다. 아래에서 새로 뽑으세요." items={queued} startLabel="풀기" />
      <DrawCard />
      <section className="card">
        <div className="card-head">
          <h2>최근 AC</h2>
          <Link to="/history" className="small">
            전체 기록 →
          </Link>
        </div>
        {solved.length === 0 ? (
          <p className="muted">아직 기록이 없습니다.</p>
        ) : (
          <ul className="rows">
            {solved.slice(0, 5).map((p) => (
              <li key={p.id} className="row">
                <ProblemLink p={p} />
                <Tier level={p.level} />
                <Tags tags={p.tags} />
                <span className="spacer" />
                <span className="muted small">{formatDate(p.solved_at)}</span>
              </li>
            ))}
          </ul>
        )}
      </section>
      <ErrorBox error={problems.error} />
    </div>
  );
}

function CurrentCard({ p }: { p?: Problem }) {
  const navigate = useNavigate();
  const open = useProblemMutation((args: { id: number; lang: Lang }) => api.openEditor(args.id, args.lang));
  const later = useProblemMutation((args: { id: number; reason: string }) => api.setStatus(args.id, "later", args.reason));
  if (!p)
    return (
      <section className="card card-current empty">
        <h2>현재 문제</h2>
        <p className="muted">풀고 있는 문제가 없습니다. 아래 목록에서 고르거나 새로 뽑으세요.</p>
      </section>
    );
  return (
    <section className="card card-current">
      <div className="muted small">현재 문제</div>
      <h1 className="current-title">
        <span className="pid">{p.id}</span> {p.title}
      </h1>
      <div className="muted small">
        {p.time_limit} · {p.memory_limit} · {p.language === "cpp" ? "C++" : "Python"} · 시작 {formatDate(p.started_at)}
        {p.watching && <span className="warn"> · AC 확인 중</span>}
      </div>
      {p.memo && <p className="memo-excerpt">📝 {p.memo}</p>}
      <div className="actions">
        <button className="btn" onClick={() => navigate(`/problems/${p.id}`)}>
          문제 · 메모
        </button>
        <button
          className="btn"
          disabled={open.isPending}
          onClick={() => open.mutate({ id: p.id, lang: p.language }, { onSuccess: openEditorResult })}
        >
          VSCode 열기
        </button>
        <button className="btn btn-primary" onClick={() => navigate(`/problems/${p.id}/submit`)}>
          제출 준비 →
        </button>
        <span className="spacer" />
        <LaterButton busy={later.isPending} onSubmit={(reason) => later.mutate({ id: p.id, reason })} />
      </div>
      <ErrorBox error={open.error ?? later.error} />
    </section>
  );
}

function ProblemList({ title, empty, items, startLabel }: { title: string; empty: string; items: Problem[]; startLabel: string }) {
  const navigate = useNavigate();
  const settings = useSettings();
  const start = useProblemMutation((args: { id: number; lang: Lang }) => api.start(args.id, args.lang));
  const skip = useProblemMutation((args: { id: number; reason: string }) => api.setStatus(args.id, "skipped", args.reason));
  const defaultLang: Lang = settings.data?.langs[0]?.id ?? "cpp";

  return (
    <section className="card">
      <div className="card-head">
        <h2>{title}</h2>
        <span className="muted small">{items.length}개</span>
      </div>
      {items.length === 0 ? (
        <p className="muted">{empty}</p>
      ) : (
        <ul className="rows">
          {items.map((p) => (
            <li key={p.id} className="row">
              <ProblemLink p={p} />
              {p.reason && <span className="muted small">— {p.reason}</span>}
              {p.memo && <span className="memo-excerpt small">📝 {p.memo}</span>}
              <span className="spacer" />
              <button
                className="btn"
                disabled={start.isPending}
                onClick={() =>
                  start.mutate(
                    { id: p.id, lang: p.status === "queued" ? defaultLang : p.language },
                    {
                      onSuccess: (r) => {
                        openEditorResult(r);
                        navigate(`/problems/${p.id}`);
                      },
                    },
                  )
                }
              >
                {startLabel}
              </button>
              <SkipButton busy={skip.isPending} onSubmit={(reason) => skip.mutate({ id: p.id, reason })} />
            </li>
          ))}
        </ul>
      )}
      <ErrorBox error={start.error ?? skip.error} />
    </section>
  );
}

function DrawCard() {
  const settings = useSettings();
  const [n, setN] = useState(3);
  const [query, setQuery] = useState("");
  const [byId, setById] = useState("");
  const draw = useProblemMutation((a: { n: number; query: string }) => api.draw(a.n, a.query || undefined));
  const add = useProblemMutation((id: number) => api.add(id));

  return (
    <section className="card">
      <div className="card-head">
        <h2>새로 뽑기</h2>
        <span className="muted small">티어와 태그는 풀기 전까지 보이지 않습니다</span>
      </div>
      <form
        className="inline-form"
        onSubmit={(e) => {
          e.preventDefault();
          draw.mutate({ n, query: query.trim() });
        }}
      >
        <label className="field">
          <span>개수</span>
          <input type="number" min={1} max={20} value={n} onChange={(e) => setN(+e.target.value)} style={{ width: 64 }} />
        </label>
        <label className="field grow">
          <span>solved.ac 쿼리</span>
          <input value={query} onChange={(e) => setQuery(e.target.value)} placeholder={settings.data?.defaultQuery ?? "*g"} />
        </label>
        <button className="btn btn-primary" disabled={draw.isPending || !settings.data?.hasKey}>
          {draw.isPending ? "뽑는 중..." : "뽑기"}
        </button>
      </form>
      {draw.data && (
        <p className="small">
          <span className="muted">{draw.data.query} →</span>{" "}
          {draw.data.added.map((p) => (
            <span key={p.id} className="chip">
              <ProblemLink p={p} /> <StatusBadge status={p.status} />
            </span>
          ))}
        </p>
      )}
      {draw.data && draw.data.failed.length > 0 && (
        <p className="small warn">
          가져오지 못한 문제: {draw.data.failed.map((f) => `${f.id} (${f.error})`).join(", ")}
        </p>
      )}
      <form
        className="inline-form"
        onSubmit={(e) => {
          e.preventDefault();
          const id = parseInt(byId, 10);
          if (id > 0) add.mutate(id, { onSuccess: () => setById("") });
        }}
      >
        <label className="field">
          <span>번호로 추가</span>
          <input value={byId} onChange={(e) => setById(e.target.value.replace(/\D/g, ""))} placeholder="1003" style={{ width: 100 }} />
        </label>
        <button className="btn" disabled={add.isPending}>
          추가
        </button>
      </form>
      <ErrorBox error={draw.error ?? add.error} />
    </section>
  );
}

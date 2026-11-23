import { useParams, useNavigate, Link } from "react-router";
import { api, openEditorResult, useDetail, useProblemMutation, useSettings, type Lang } from "../api";
import { ErrorBox, LaterButton, SkipButton, StatementView, StatusBadge, Tags, Tier } from "../components";
import MemoEditor from "../Memo";

export default function ProblemPage() {
  const id = Number(useParams().id);
  const navigate = useNavigate();
  const detail = useDetail(id);
  const settings = useSettings();
  const start = useProblemMutation((lang: Lang) => api.start(id, lang));
  const later = useProblemMutation((reason: string) => api.setStatus(id, "later", reason));
  const skip = useProblemMutation((reason: string) => api.setStatus(id, "skipped", reason));
  const back = useProblemMutation(() => api.setStatus(id, "queued"));

  if (detail.isPending) return <p className="muted">불러오는 중...</p>;
  if (detail.error) return <ErrorBox error={detail.error} />;
  const { problem: p, statement: st, langs } = detail.data;
  const solving = p.status === "current";

  return (
    <div className="stack">
      <section className="card">
        <div className="problem-head">
          <div>
            <h1>
              <span className="pid">{p.id}</span> {p.title}
            </h1>
            <div className="meta-line">
              <StatusBadge status={p.status} />
              <Tier level={p.level} />
              <span className="muted small">
                {p.time_limit} · {p.memory_limit}
              </span>
              <a className="small" href={`https://www.acmicpc.net/problem/${p.id}`} target="_blank" rel="noreferrer">
                BOJ에서 보기 ↗
              </a>
            </div>
            <Tags tags={p.tags} />
            {p.reason && <p className="muted small">사유: {p.reason}</p>}
          </div>
        </div>
        <div className="actions">
          {(settings.data?.langs ?? []).map((l) => (
            <button
              key={l.id}
              className={`btn ${solving && p.language === l.id ? "btn-primary" : ""}`}
              disabled={start.isPending}
              title={langs.includes(l.id) ? "작성 중인 파일을 연다" : "템플릿으로 파일을 만들어 연다"}
              onClick={() => start.mutate(l.id, { onSuccess: openEditorResult })}
            >
              {solving && p.language === l.id ? `VSCode 열기 (${l.label})` : `${l.label}로 풀기`}
              {langs.includes(l.id) && !(solving && p.language === l.id) ? " · 이어서" : ""}
            </button>
          ))}
          {langs.length > 0 && (
            <button className="btn btn-primary" onClick={() => navigate(`/problems/${id}/submit`)}>
              제출 준비 →
            </button>
          )}
          <span className="spacer" />
          {["current", "paused", "queued"].includes(p.status) && (
            <LaterButton busy={later.isPending} onSubmit={(r) => later.mutate(r)} />
          )}
          {p.status !== "skipped" && p.status !== "solved" && <SkipButton busy={skip.isPending} onSubmit={(r) => skip.mutate(r)} />}
          {(p.status === "skipped" || p.status === "later") && (
            <button className="btn" onClick={() => back.mutate()}>
              다시 대기로
            </button>
          )}
        </div>
        <p className="muted small">
          작업 폴더: <code>{detail.data.dir}</code> · 직접 만든 테스트는 <code>inputs/이름.in</code> (정답은{" "}
          <code>outputs/이름.out</code>, 없으면 채점 없이 실행만)
        </p>
        <ErrorBox error={start.error ?? later.error ?? skip.error} />
      </section>

      <div className="with-aside">
        <section className="card">{st ? <StatementView st={st} /> : <p className="muted">지문이 없습니다.</p>}</section>
        <aside className="aside">
          <MemoEditor id={id} />
        </aside>
      </div>

      <p>
        <Link to="/">← 목록</Link>
      </p>
    </div>
  );
}

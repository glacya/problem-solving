import { useState } from "react";
import { api, useProblemMutation, useProblems, type Status } from "../api";
import { ErrorBox, ProblemLink, StatusBadge, Tags, Tier, formatDate } from "../components";

const FILTERS: { key: "solved" | "later" | "paused" | "skipped" | "all"; label: string }[] = [
  { key: "solved", label: "AC" },
  { key: "later", label: "나중에 풀기" },
  { key: "paused", label: "풀다 만" },
  { key: "skipped", label: "스킵" },
  { key: "all", label: "전체" },
];

export default function History() {
  const problems = useProblems();
  const [filter, setFilter] = useState<(typeof FILTERS)[number]["key"]>("solved");
  const setStatus = useProblemMutation((a: { id: number; status: Status }) => api.setStatus(a.id, a.status));
  const remove = useProblemMutation((id: number) => api.remove(id));

  const list = (problems.data ?? [])
    .filter((p) => filter === "all" || p.status === filter)
    .sort((a, b) => (b.solved_at ?? b.updated_at).localeCompare(a.solved_at ?? a.updated_at));
  const solvedCount = (problems.data ?? []).filter((p) => p.status === "solved").length;

  return (
    <div className="stack">
      <section className="card">
        <div className="card-head">
          <h2>기록</h2>
          <span className="muted small">AC {solvedCount}문제</span>
        </div>
        <div className="tabs">
          {FILTERS.map((f) => (
            <button key={f.key} className={`tab ${filter === f.key ? "on" : ""}`} onClick={() => setFilter(f.key)}>
              {f.label}
            </button>
          ))}
        </div>
        {list.length === 0 ? (
          <p className="muted">기록이 없습니다.</p>
        ) : (
          <div className="table-wrap">
            <table className="table">
              <thead>
                <tr>
                  <th>문제</th>
                  <th>상태</th>
                  <th>티어 / 태그</th>
                  <th>사유 · 메모</th>
                  <th>날짜</th>
                  <th />
                </tr>
              </thead>
              <tbody>
                {list.map((p) => (
                  <tr key={p.id}>
                    <td>
                      <ProblemLink p={p} />
                    </td>
                    <td>
                      <StatusBadge status={p.status} />
                    </td>
                    <td>
                      {p.status === "solved" ? (
                        <>
                          <Tier level={p.level} /> <Tags tags={p.tags} />
                        </>
                      ) : (
                        <span className="muted small">풀면 공개</span>
                      )}
                    </td>
                    <td className="small">
                      {p.reason}
                      {p.memo && <div className="memo-excerpt">📝 {p.memo}</div>}
                    </td>
                    <td className="muted small nowrap">{formatDate(p.solved_at ?? p.updated_at)}</td>
                    <td className="nowrap">
                      {(p.status === "skipped" || p.status === "later" || p.status === "paused") && (
                        <button className="btn btn-ghost" onClick={() => setStatus.mutate({ id: p.id, status: "queued" })}>
                          다시 대기로
                        </button>
                      )}
                      {p.status !== "solved" && (
                        <button
                          className="btn btn-ghost"
                          onClick={() => confirm(`${p.id} 를 기록에서 지울까요?`) && remove.mutate(p.id)}
                        >
                          삭제
                        </button>
                      )}
                    </td>
                  </tr>
                ))}
              </tbody>
            </table>
          </div>
        )}
        <ErrorBox error={problems.error ?? setStatus.error ?? remove.error} />
      </section>
    </div>
  );
}

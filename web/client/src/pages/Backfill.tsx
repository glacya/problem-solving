// 푼 문제 중 solution/ 에 소스가 없는 것. solved.ac 를 여러 번 불러 오래 걸리므로 이 페이지에서만 받는다.
import { useMemo, useState } from "react";
import { Link } from "react-router";
import { useMissing } from "../api";
import { ErrorBox, Tier } from "../components";

type Sort = "id" | "level";

export default function Backfill() {
  const missing = useMissing();
  const [filter, setFilter] = useState("");
  const [sort, setSort] = useState<Sort>("id");

  const list = useMemo(() => {
    const f = filter.trim().toLowerCase();
    const rows = (missing.data?.problems ?? []).filter(
      (p) => !f || String(p.id).startsWith(f) || p.title.toLowerCase().includes(f),
    );
    return sort === "id" ? rows : [...rows].sort((a, b) => b.level - a.level || a.id - b.id);
  }, [missing.data, filter, sort]);

  const d = missing.data;
  return (
    <div className="stack">
      <section className="card">
        <div className="card-head">
          <h2>소스 모으기</h2>
          {d && (
            <span className="muted small">
              푼 문제 {d.solved} · 소스 있음 {d.local} · <b>없음 {d.problems.length}</b>
            </span>
          )}
          <button className="btn" onClick={() => missing.refetch()} disabled={missing.isFetching}>
            {missing.isFetching ? "불러오는 중…" : "새로 불러오기"}
          </button>
          <Link className="btn btn-primary" to="/backfill/add">
            번호로 추가
          </Link>
        </div>
        <p className="muted small">
          solved.ac 에서 푼 문제(BOJ 에서 동기화된 AC)를 받아 <code>solution/boj/</code> 에 소스가 없는 문제를 보여 줍니다. 문제를
          누르면 소스를 추가하는 화면으로 갑니다. BOJ 에 바로 푼 문제가 solved.ac 에 아직 없다면 solved.ac 에서 프로필을
          갱신하세요.
        </p>

        <div className="inline-form">
          <label className="field grow">
            찾기
            <input value={filter} onChange={(e) => setFilter(e.target.value)} placeholder="번호 또는 제목" />
          </label>
          <div className="tabs">
            <button className={`tab ${sort === "id" ? "on" : ""}`} onClick={() => setSort("id")}>
              번호순
            </button>
            <button className={`tab ${sort === "level" ? "on" : ""}`} onClick={() => setSort("level")}>
              난이도순
            </button>
          </div>
        </div>

        {missing.isPending ? (
          <p className="muted">solved.ac 에서 푼 문제 목록을 받는 중입니다. 1000문제면 몇 초 걸립니다…</p>
        ) : d && d.problems.length === 0 ? (
          <p className="ok-box">푼 문제의 소스가 모두 있습니다.</p>
        ) : (
          d && (
            <div className="table-wrap">
              <table className="table">
                <thead>
                  <tr>
                    <th>문제</th>
                    <th>난이도</th>
                    <th />
                  </tr>
                </thead>
                <tbody>
                  {list.map((p) => (
                    <tr key={p.id}>
                      <td>
                        <Link to={`/backfill/add?id=${p.id}`} className="plink">
                          <span className="pid">{p.id}</span> {p.title}
                        </Link>
                      </td>
                      <td>
                        <Tier level={p.level} />
                      </td>
                      <td className="nowrap" style={{ textAlign: "right" }}>
                        <a href={`https://www.acmicpc.net/problem/${p.id}`} target="_blank" rel="noreferrer" className="small">
                          BOJ ↗
                        </a>
                      </td>
                    </tr>
                  ))}
                </tbody>
              </table>
              {list.length === 0 && <p className="muted">맞는 문제가 없습니다.</p>}
            </div>
          )
        )}
        <ErrorBox error={missing.error} />
      </section>
    </div>
  );
}

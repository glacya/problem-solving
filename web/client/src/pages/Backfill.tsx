// 푼 문제 중 solution/ 에 소스가 없는 것. solved.ac 목록은 서버가 캐시하고, '새로 불러오기' 때만 다시 받는다.
import { useMemo, useState } from "react";
import { Link } from "react-router";
import { useMutation, useQueryClient } from "@tanstack/react-query";
import { api, statusUrl, useMissing, useSettings } from "../api";
import { BookmarkletLink, ErrorBox, Tier, formatDate } from "../components";

type Sort = "id" | "level";

export default function Backfill() {
  const missing = useMissing();
  const settings = useSettings();
  const handle = settings.data?.handle ?? "";
  const qc = useQueryClient();
  const refresh = useMutation({
    mutationFn: () => api.missing(true),
    onSuccess: (d) => qc.setQueryData(["backfill-missing"], d),
  });
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
          <button
            className="btn"
            onClick={() => refresh.mutate()}
            disabled={refresh.isPending}
            title={d ? `solved.ac 목록: ${formatDate(d.fetchedAt)} 에 받음` : undefined}
          >
            {refresh.isPending ? "solved.ac 에서 받는 중…" : "새로 불러오기"}
          </button>
          <Link className="btn btn-primary" to="/backfill/add">
            번호로 추가
          </Link>
        </div>
        <p className="muted small">
          solved.ac 에서 받은 푼 문제(BOJ 에서 동기화된 AC) 중 <code>solution/boj/</code> 에 소스가 없는 문제입니다. 목록은 서버에
          저장해 두고, 새로 푼 문제를 반영하려면 '새로 불러오기'를 누르세요.
          {d && ` (마지막으로 받은 때: ${formatDate(d.fetchedAt)})`}
        </p>

        <div className="howto small">
          <b>여러 문제를 연달아 넣기</b>
          <ol>
            <li>
              <BookmarkletLink /> 를 북마크바로 끌어다 놓습니다 (한 번만).
            </li>
            <li>
              아래 목록에서 아무 문제의 <b>내 제출 ↗</b> 을 엽니다. BOJ 에 그 문제의 내 맞은 제출 목록이 열립니다.
            </li>
            <li>
              넣을 제출의 소스 페이지를 열고 북마크를 누릅니다. 바로 저장되고, 같은 탭이 <b>다음 문제의 내 제출 목록</b>으로
              넘어갑니다. 이걸 반복합니다.
            </li>
            <li>넣지 않을 문제는 제출 목록에서 북마크를 누르면 건너뜁니다.</li>
          </ol>
          <span className="muted">
            처음 누를 때 Chrome 이 "로컬 네트워크의 기기에 접근"을 물으면 허용하세요. 저장은 커밋하지 않습니다. AC 가 아니거나 다른
            사람의 제출이면 저장 전에 묻습니다.
          </span>
        </div>

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
                        {handle && (
                          <a href={statusUrl(p.id, handle)} target="boj" className="small">
                            내 제출 ↗
                          </a>
                        )}{" "}
                        <a href={`https://www.acmicpc.net/problem/${p.id}`} target="_blank" rel="noreferrer" className="small muted">
                          문제 ↗
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
        <ErrorBox error={missing.error ?? refresh.error} />
      </section>
    </div>
  );
}

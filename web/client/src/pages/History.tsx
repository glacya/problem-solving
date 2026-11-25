// 문제 풀이 기록. 상태, 기간(일/주/월), 검색, 티어로 거르고 20개씩 페이지로 나눠 본다.
// 필터는 주소(?status=&period=&date=&q=&tier=&page=)에 두어서 뒤로 가기와 새로고침에도 그대로 남는다.
import { useMemo } from "react";
import { useSearchParams } from "react-router";
import { useMutation, useQuery, useQueryClient } from "@tanstack/react-query";
import { api, useProblemMutation, useProblems, type Problem, type Status } from "../api";
import { ErrorBox, ProblemLink, StatusBadge, Tags, Tier, formatDate, formatDay } from "../components";

const PAGE_SIZE = 20;

const STATUS_FILTERS = [
  { key: "solved", label: "AC" },
  { key: "later", label: "나중에 풀기" },
  { key: "paused", label: "풀다 만" },
  { key: "skipped", label: "스킵" },
  { key: "all", label: "전체" },
] as const;
type StatusFilter = (typeof STATUS_FILTERS)[number]["key"];

const PERIODS = [
  { key: "all", label: "전체 기간" },
  { key: "day", label: "일" },
  { key: "week", label: "주" },
  { key: "month", label: "월" },
] as const;
type Period = (typeof PERIODS)[number]["key"];

const TIERS = ["Bronze", "Silver", "Gold", "Platinum", "Diamond", "Ruby"];
const WEEKDAYS = ["일", "월", "화", "수", "목", "금", "토"];

// ---- 날짜 (모두 로컬 날짜, "YYYY-MM-DD") ----

const pad = (n: number) => String(n).padStart(2, "0");
const ymd = (d: Date) => `${d.getFullYear()}-${pad(d.getMonth() + 1)}-${pad(d.getDate())}`;
function parse(s: string) {
  const [y, m, d] = s.split("-").map(Number);
  return new Date(y, m - 1, d);
}
const todayStr = () => ymd(new Date());

/** 기간 [시작, 끝] (끝 포함). 주는 월요일부터. */
function range(period: Period, anchor: string): [string, string] | null {
  const d = parse(anchor);
  if (period === "day") return [anchor, anchor];
  if (period === "week") {
    const start = new Date(d);
    start.setDate(d.getDate() - ((d.getDay() + 6) % 7));
    const end = new Date(start);
    end.setDate(start.getDate() + 6);
    return [ymd(start), ymd(end)];
  }
  if (period === "month") {
    return [ymd(new Date(d.getFullYear(), d.getMonth(), 1)), ymd(new Date(d.getFullYear(), d.getMonth() + 1, 0))];
  }
  return null;
}

function shift(period: Period, anchor: string, dir: number) {
  const d = parse(anchor);
  if (period === "day") d.setDate(d.getDate() + dir);
  if (period === "week") d.setDate(d.getDate() + 7 * dir);
  if (period === "month") return ymd(new Date(d.getFullYear(), d.getMonth() + dir, 1));
  return ymd(d);
}

function rangeLabel(period: Period, anchor: string) {
  const r = range(period, anchor);
  if (!r) return "";
  const [s, e] = r;
  if (period === "day") return `${s} (${WEEKDAYS[parse(s).getDay()]})`;
  if (period === "week") return `${s} ~ ${e.slice(5)}`;
  return `${s.slice(0, 4)}년 ${Number(s.slice(5, 7))}월`;
}

/** 기록의 날짜: AC 는 푼 날짜, 나머지는 마지막으로 바뀐 날. */
const dayOf = (p: Problem) => (p.status === "solved" && p.solved_at ? p.solved_at : p.updated_at).slice(0, 10);

function tierGroup(level: number | null) {
  if (level === null) return null;
  return level === 0 ? "unrated" : TIERS[Math.floor((level - 1) / 5)];
}

export default function History() {
  const problems = useProblems();
  const [params, setParams] = useSearchParams();
  const setStatus = useProblemMutation((a: { id: number; status: Status }) => api.setStatus(a.id, a.status));
  const remove = useProblemMutation((id: number) => api.remove(id));

  const status = (params.get("status") ?? "solved") as StatusFilter;
  const period = (params.get("period") ?? "all") as Period;
  const anchor = /^\d{4}-\d{2}-\d{2}$/.test(params.get("date") ?? "") ? params.get("date")! : todayStr();
  const q = params.get("q") ?? "";
  const tier = params.get("tier") ?? "";
  const page = Math.max(1, Number(params.get("page")) || 1);

  /** 필터를 바꾸면 첫 페이지로 (page 만 바꿀 때는 그대로). */
  const update = (patch: Record<string, string | null>) => {
    const next = new URLSearchParams(params);
    for (const [k, v] of Object.entries(patch)) {
      if (v === null || v === "") next.delete(k);
      else next.set(k, v);
    }
    if (!("page" in patch)) next.delete("page");
    setParams(next, { replace: "q" in patch });
  };

  const all = problems.data ?? [];
  const filtered = useMemo(() => {
    const r = range(period, anchor);
    const needle = q.trim().toLowerCase();
    return all
      .filter((p) => status === "all" || p.status === status)
      .filter((p) => !r || (dayOf(p) >= r[0] && dayOf(p) <= r[1]))
      .filter((p) => !needle || String(p.id).startsWith(needle) || p.title.toLowerCase().includes(needle))
      .filter((p) => !tier || tierGroup(p.level) === tier)
      .sort((a, b) => dayOf(b).localeCompare(dayOf(a)) || b.updated_at.localeCompare(a.updated_at) || b.id - a.id);
  }, [all, status, period, anchor, q, tier]);

  const pages = Math.max(1, Math.ceil(filtered.length / PAGE_SIZE));
  const cur = Math.min(page, pages);
  const list = filtered.slice((cur - 1) * PAGE_SIZE, cur * PAGE_SIZE);
  const solvedCount = all.filter((p) => p.status === "solved").length;
  const activeDays = new Set(filtered.map(dayOf)).size;

  return (
    <div className="stack">
      <section className="card">
        <div className="card-head">
          <h2>기록</h2>
          <span className="muted small">AC {solvedCount}문제</span>
        </div>

        <div className="tabs">
          {STATUS_FILTERS.map((f) => (
            <button key={f.key} className={`tab ${status === f.key ? "on" : ""}`} onClick={() => update({ status: f.key })}>
              {f.label}
            </button>
          ))}
        </div>

        <div className="filters">
          <div className="tabs">
            {PERIODS.map((p) => (
              <button
                key={p.key}
                className={`tab ${period === p.key ? "on" : ""}`}
                onClick={() => update({ period: p.key === "all" ? null : p.key })}
              >
                {p.label}
              </button>
            ))}
          </div>
          {period !== "all" && (
            <div className="period-nav">
              <button className="btn" onClick={() => update({ date: shift(period, anchor, -1) })} aria-label="이전">
                ‹
              </button>
              <input type="date" value={anchor} onChange={(e) => e.target.value && update({ date: e.target.value })} />
              <span className="period-label">{rangeLabel(period, anchor)}</span>
              <button className="btn" onClick={() => update({ date: shift(period, anchor, 1) })} aria-label="다음">
                ›
              </button>
              <button className="btn btn-ghost" onClick={() => update({ date: null })}>
                오늘
              </button>
            </div>
          )}
        </div>

        <div className="filters">
          <input
            className="grow"
            value={q}
            onChange={(e) => update({ q: e.target.value })}
            placeholder="번호 또는 제목으로 찾기"
          />
          <select value={tier} onChange={(e) => update({ tier: e.target.value })} aria-label="티어">
            <option value="">모든 티어</option>
            {TIERS.map((t) => (
              <option key={t} value={t}>
                {t}
              </option>
            ))}
            <option value="unrated">Unrated</option>
          </select>
        </div>

        <p className="muted small">
          {filtered.length}문제
          {period !== "all" && activeDays > 0 && ` · ${activeDays}일`}
          {tier && status !== "solved" && " · 티어는 푼 문제만 알 수 있습니다"}
        </p>

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
                    <td className="muted small nowrap">
                      {p.status === "solved" ? formatDay(p.solved_at) : formatDate(p.updated_at)}
                    </td>
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
        {pages > 1 && <Pager page={cur} pages={pages} go={(n) => update({ page: n === 1 ? null : String(n) })} />}
        <ErrorBox error={problems.error ?? setStatus.error ?? remove.error} />
      </section>
      <CleanupCard />
    </div>
  );
}

function Pager({ page, pages, go }: { page: number; pages: number; go: (n: number) => void }) {
  const nums: (number | "…")[] = [];
  for (let n = 1; n <= pages; n++) {
    if (n === 1 || n === pages || Math.abs(n - page) <= 2) nums.push(n);
    else if (nums[nums.length - 1] !== "…") nums.push("…");
  }
  return (
    <nav className="pager">
      <button className="btn" disabled={page === 1} onClick={() => go(page - 1)} aria-label="이전 페이지">
        ‹
      </button>
      {nums.map((n, i) =>
        n === "…" ? (
          <span key={`gap${i}`} className="muted">
            …
          </span>
        ) : (
          <button key={n} className={`tab ${n === page ? "on" : ""}`} onClick={() => go(n)}>
            {n}
          </button>
        ),
      )}
      <button className="btn" disabled={page === pages} onClick={() => go(page + 1)} aria-label="다음 페이지">
        ›
      </button>
    </nav>
  );
}

/** 풀이 폴더 정리. 서버도 뜰 때와 몇 시간마다 자동으로 정리한다. */
function CleanupCard() {
  const qc = useQueryClient();
  const preview = useQuery({ queryKey: ["cleanup"], queryFn: api.cleanupPreview });
  const run = useMutation({
    mutationFn: api.cleanup,
    onSuccess: () => {
      qc.invalidateQueries({ queryKey: ["cleanup"] });
      qc.invalidateQueries({ queryKey: ["problem"] });
    },
  });
  const c = preview.data?.candidates ?? [];
  return (
    <section className="card">
      <div className="card-head">
        <h2>풀이 폴더 정리</h2>
        <button className="btn" disabled={c.length === 0 || run.isPending} onClick={() => run.mutate()}>
          {run.isPending ? "정리하는 중…" : `지금 정리 (${c.length})`}
        </button>
      </div>
      <p className="muted small">
        AC 를 받고 마지막 제출일로부터 {preview.data?.keepDays ?? 3}일 이상 지난 문제의 <code>problems/&lt;번호&gt;/</code> 를
        지웁니다. 소스가 <code>solution/</code> 에 있는 문제만 지우고, 메모(<code>memo.md</code>)는 남깁니다. 서버가 뜰 때와 6시간마다
        자동으로 정리합니다.
      </p>
      {c.length > 0 && (
        <p className="small">
          정리 대상:{" "}
          {c.map((x) => (
            <span key={x.id} className="chip">
              <span className="pid">{x.id}</span> {x.title} <span className="muted">({x.last})</span>
            </span>
          ))}
        </p>
      )}
      {run.data && <div className="ok-box">{run.data.removed.length}개 폴더를 정리했습니다.</div>}
      <ErrorBox error={preview.error ?? run.error} />
    </section>
  );
}

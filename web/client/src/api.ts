// 백엔드(/api) 타입과 호출. 서버 상태는 TanStack Query 로 다룬다.
import { useMutation, useQuery, useQueryClient } from "@tanstack/react-query";

export type Status = "queued" | "current" | "paused" | "later" | "skipped" | "solved";
export type Lang = "cpp" | "py";

export interface Problem {
  id: number;
  title: string;
  status: Status;
  reason: string;
  source: string;
  language: Lang;
  time_limit: string;
  memory_limit: string;
  added_at: string;
  updated_at: string;
  started_at: string | null;
  submitted_at: string | null;
  solved_at: string | null; // 푼 날짜 YYYY-MM-DD (시각은 저장하지 않는다)
  level: number | null; // 푼 문제만 (안 푼 문제는 서버가 보내지 않는다)
  tags: string[] | null; // 푼 문제만
  watching: boolean; // 서버가 AC 를 확인하는 중
  watch_until: string | null;
  commit_log: string;
  memo: string; // 메모 첫 줄
}

export interface Memo {
  text: string;
  updatedAt: string | null;
  path: string;
}

export interface Statement {
  title: string;
  timeLimit: string;
  memoryLimit: string;
  sections: { key: string; name: string; html: string; text: string }[];
  samples: { input: string; output: string }[];
}

export interface Detail {
  problem: Problem;
  statement: Statement | null;
  tests: { name: string; hasOutput: boolean }[];
  langs: Lang[];
  dir: string;
  editorUri: string;
}

export interface Settings {
  handle: string;
  hasKey: boolean;
  defaultQuery: string;
  langs: { id: Lang; label: string; boj: string }[];
  sourceLangs: { id: string; label: string }[]; // 예전 소스를 추가할 때 고르는 언어 (확장자)
}

/** 푼 문제 하나 (solved.ac 메타). 푼 문제라 티어와 태그가 있다. */
export interface SolvedMeta {
  id: number;
  title: string;
  level: number;
  tags: string[];
  solved_count: number | null;
}

export interface Missing {
  handle: string;
  solved: number; // 푼 문제 수
  local: number; // 그중 solution/ 에 소스가 있는 수
  fetchedAt: string; // solved.ac 에서 푼 문제 목록을 받은 시각 (서버 캐시)
  problems: SolvedMeta[]; // 소스가 없는 문제
}

/** 서버가 해석한 BOJ 소스 페이지. */
export interface BojPage {
  submissionId: number | null;
  user: string;
  problemId: number;
  problemTitle: string;
  result: string;
  accepted: boolean;
  language: string; // BOJ 표기 ("C++20")
  ext: string | null; // solution/ 확장자
  date: string; // 제출 날짜
  code: string;
  warnings: string[];
}

export interface BackfillInfo {
  id: number;
  title: string;
  solved: boolean;
  level: number | null; // 안 푼 문제는 서버가 보내지 않는다
  tags: string[] | null;
  files: string[]; // 이미 있는 solution 파일
  statusUrl: string | null; // BOJ 에서 내 AC 제출 보기 (브라우저로 직접 연다)
}

export interface Bundle {
  lang: Lang;
  code: string;
  used: string[];
  lines: number;
  bytes: number;
}

export interface TestResult {
  name: string;
  status: "PASS" | "WA" | "RE" | "TLE" | "SKIP";
  ms: number;
  memKb: number;
  reason: string;
  input: string;
  expected: string | null;
  output: string;
  stderr: string;
}

export interface JudgeResult {
  lang: Lang;
  limitMs: number;
  compile: { ok: boolean; ms: number; log: string };
  results: TestResult[];
  bundle?: { lines: number; bytes: number; used: string[] };
}

export class ApiError extends Error {}

async function call<T>(path: string, init?: { method?: string; body?: unknown }): Promise<T> {
  const res = await fetch(`/api${path}`, {
    method: init?.method ?? (init?.body === undefined ? "GET" : "POST"),
    headers: { "Content-Type": "application/json" },
    body: init?.body === undefined ? undefined : JSON.stringify(init.body),
  });
  const data = await res.json().catch(() => ({}));
  if (!res.ok) throw new ApiError(typeof data.detail === "string" ? data.detail : `HTTP ${res.status}`);
  return data as T;
}

export const api = {
  settings: () => call<Settings>("/settings"),
  problems: () => call<Problem[]>("/problems"),
  detail: (id: number) => call<Detail>(`/problems/${id}`),
  draw: (n: number, query?: string) =>
    call<{ query: string; added: Problem[]; failed: { id: number; error: string }[] }>("/draw", { body: { n, query } }),
  add: (id: number) => call<Problem>("/problems", { body: { id } }),
  setStatus: (id: number, status: Status, reason?: string) =>
    call<Problem>(`/problems/${id}/status`, { body: { status, reason } }),
  remove: (id: number) => call<{ ok: boolean }>(`/problems/${id}`, { method: "DELETE", body: {} }),
  start: (id: number, lang: Lang) =>
    call<{ problem: Problem; opened: boolean; editorUri: string; path: string }>(`/problems/${id}/start`, { body: { lang } }),
  openEditor: (id: number, lang: Lang) =>
    call<{ opened: boolean; editorUri: string; path: string }>(`/problems/${id}/open-editor`, { body: { lang } }),
  bundle: (id: number, lang: Lang) => call<Bundle>(`/problems/${id}/bundle?lang=${lang}`),
  judge: (id: number, lang: Lang) => call<JudgeResult>(`/problems/${id}/judge`, { body: { lang } }),
  submit: (id: number, lang: Lang) => call<Bundle & { submitUrl: string }>(`/problems/${id}/submit`, { body: { lang } }),
  check: (id: number) =>
    call<{ solved: boolean; message?: string; problem: Problem; commit?: { ok: boolean; log: string } }>(
      `/problems/${id}/check`,
      { body: {} },
    ),
  markSolved: (id: number, lang: Lang) =>
    call<{ solved: boolean; problem: Problem; commit?: { ok: boolean; log: string } }>(`/problems/${id}/mark-solved`, {
      body: { lang },
    }),
  watch: (id: number) => call<Problem>(`/problems/${id}/watch`, { body: {} }),
  unwatch: (id: number) => call<Problem>(`/problems/${id}/watch`, { method: "DELETE", body: {} }),
  memo: (id: number) => call<Memo>(`/problems/${id}/memo`),
  saveMemo: (id: number, text: string) => call<Memo>(`/problems/${id}/memo`, { method: "PUT", body: { text } }),
  health: () => call<{ ok: boolean }>("/health"),
  missing: (refresh = false) => call<Missing>(`/backfill/missing${refresh ? "?refresh=true" : ""}`),
  parsePage: (html: string) => call<BojPage>("/backfill/parse", { body: { html } }),
  backfillInfo: (id: number) => call<BackfillInfo>(`/backfill/${id}`),
  backfill: (id: number, date: string, lang: string, code: string) =>
    call<{ id: number; path: string; history: string; historyFiles: string[] }>(`/backfill/${id}`, {
      body: { date, lang, code },
    }),
  cleanupPreview: () =>
    call<{ keepDays: number; candidates: { id: number; title: string; last: string }[] }>("/cleanup"),
  cleanup: () => call<{ removed: { id: number; title: string; last: string }[] }>("/cleanup", { body: {} }),
};

/** 서버 연결 상태. 끊기면 5초마다 다시 확인한다. */
export const useHealth = () =>
  useQuery({
    queryKey: ["health"],
    queryFn: api.health,
    retry: false,
    refetchInterval: (q) => (q.state.status === "error" ? 5_000 : 30_000),
    refetchIntervalInBackground: true,
  });

export const useSettings = () => useQuery({ queryKey: ["settings"], queryFn: api.settings, staleTime: 60_000 });
export const useProblems = () => useQuery({ queryKey: ["problems"], queryFn: api.problems });
export const useDetail = (id: number) => useQuery({ queryKey: ["problem", id], queryFn: () => api.detail(id) });

/** 소스가 없는 푼 문제. 서버가 solved.ac 목록을 캐시하므로 빠르다 (처음 한 번과 '새로 불러오기' 때만 몇 초).
 *  북마클릿이 다른 탭에서 저장하므로 창으로 돌아오면 다시 받는다. */
export const useMissing = () =>
  useQuery({ queryKey: ["backfill-missing"], queryFn: () => api.missing(), retry: false });

/** BOJ 채점 현황: 그 문제의 내 맞은 제출. */
export const statusUrl = (id: number, handle: string) =>
  `https://www.acmicpc.net/status?problem_id=${id}&user_id=${encodeURIComponent(handle)}&result_id=4`;

/** 문제 상태를 바꾸는 요청 뒤에 목록과 상세를 다시 불러온다. */
export function useProblemMutation<A, R>(fn: (a: A) => Promise<R>) {
  const qc = useQueryClient();
  return useMutation({
    mutationFn: fn,
    onSuccess: () => {
      qc.invalidateQueries({ queryKey: ["problems"] });
      qc.invalidateQueries({ queryKey: ["problem"] });
    },
  });
}

/** VSCode 로 열기: 서버가 직접 열지 못하면 vscode:// 링크로 연다. */
export function openEditorResult(r: { opened: boolean; editorUri: string }) {
  if (!r.opened) window.location.href = r.editorUri;
}

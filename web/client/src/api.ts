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
  solved_at: string | null;
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

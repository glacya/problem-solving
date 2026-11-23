// 문제별 메모. 서버의 problems/<번호>/memo.md 에 자동 저장된다 (VSCode 에서 고쳐도 된다).
// 서버가 꺼져 있으면 브라우저(localStorage)에 임시로 두었다가, 서버가 돌아오면 저장한다.
import { useEffect, useRef, useState } from "react";
import { useQuery, useQueryClient } from "@tanstack/react-query";
import { api, useHealth } from "./api";

type Draft = { text: string; at: number };
const key = (id: number) => `ps-memo-draft-${id}`;

function readDraft(id: number): Draft | null {
  try {
    const raw = localStorage.getItem(key(id));
    return raw ? (JSON.parse(raw) as Draft) : null;
  } catch {
    return null;
  }
}

function writeDraft(id: number, d: Draft | null) {
  try {
    if (d) localStorage.setItem(key(id), JSON.stringify(d));
    else localStorage.removeItem(key(id));
  } catch {
    /* 저장소를 못 쓰는 환경이면 임시 보관 없이 진행 */
  }
}

type SaveState = "idle" | "dirty" | "saving" | "saved" | "offline";

export default function MemoEditor({ id, compact }: { id: number; compact?: boolean }) {
  const qc = useQueryClient();
  const health = useHealth();
  // 창으로 돌아올 때 다시 불러와, VSCode 에서 memo.md 를 고친 것도 반영한다.
  const memo = useQuery({ queryKey: ["memo", id], queryFn: () => api.memo(id), refetchOnWindowFocus: true });
  const [text, setText] = useState<string | null>(null);
  const [state, setState] = useState<SaveState>("idle");
  const timer = useRef<number | undefined>(undefined);
  const latest = useRef("");
  const lastServer = useRef<string | null>(null);

  // 웹에서 고치던 내용이 없을 때만 서버(파일) 쪽 변경을 받아들인다.
  useEffect(() => {
    if (!memo.data) return;
    const server = memo.data.text;
    if (text !== null && (state === "idle" || state === "saved") && latest.current === lastServer.current) {
      setText(server);
      latest.current = server;
    }
    lastServer.current = server;
    // eslint-disable-next-line react-hooks/exhaustive-deps
  }, [memo.data?.text, memo.data?.updatedAt]);

  // 처음 불러올 때: 저장 못 한 임시본이 있으면 그것을 쓰고 곧 저장한다.
  useEffect(() => {
    if (memo.data && text === null) {
      const draft = readDraft(id);
      if (draft && draft.text !== memo.data.text) {
        setText(draft.text);
        latest.current = draft.text;
        setState("dirty");
        schedule(0);
      } else {
        setText(memo.data.text);
        latest.current = memo.data.text;
        writeDraft(id, null);
      }
    }
    // eslint-disable-next-line react-hooks/exhaustive-deps
  }, [memo.data]);

  // 처음부터 서버에 연결되지 않았다면: 임시본이 있을 때만 편집을 허용한다.
  // (서버 메모를 모르는 채로 빈 칸에 쓰면, 나중에 저장할 때 기존 메모를 덮어쓰게 된다.)
  useEffect(() => {
    if (memo.isError && text === null) {
      const draft = readDraft(id);
      if (draft) {
        setText(draft.text);
        latest.current = draft.text;
        setState("offline");
      }
    }
    // eslint-disable-next-line react-hooks/exhaustive-deps
  }, [memo.isError]);

  // 서버가 돌아오면 메모를 다시 불러오고, 밀린 메모를 저장한다.
  useEffect(() => {
    if (!health.isSuccess) return;
    if (memo.isError) memo.refetch();
    if (state === "offline") schedule(0);
    // eslint-disable-next-line react-hooks/exhaustive-deps
  }, [health.isSuccess, health.dataUpdatedAt]);

  useEffect(() => () => window.clearTimeout(timer.current), []);

  function schedule(ms: number) {
    window.clearTimeout(timer.current);
    timer.current = window.setTimeout(save, ms);
  }

  async function save() {
    const value = latest.current;
    setState("saving");
    try {
      const saved = await api.saveMemo(id, value);
      lastServer.current = saved.text;
      qc.setQueryData(["memo", id], saved);
      qc.invalidateQueries({ queryKey: ["problems"] });
      if (latest.current === value) {
        writeDraft(id, null);
        setState("saved");
      } else {
        schedule(500); // 저장하는 사이에 더 고쳤다
      }
    } catch {
      setState("offline"); // 임시본은 이미 localStorage 에 있다
    }
  }

  function onChange(v: string) {
    setText(v);
    latest.current = v;
    writeDraft(id, { text: v, at: Date.now() });
    setState("dirty");
    schedule(800);
  }

  const label: Record<SaveState, string> = {
    idle: memo.data?.updatedAt ? "저장됨" : "",
    dirty: "고치는 중...",
    saving: "저장 중...",
    saved: "저장됨",
    offline: "서버에 연결할 수 없어 브라우저에 임시 저장했습니다. 서버가 돌아오면 저장합니다.",
  };

  return (
    <section className="card memo">
      <div className="card-head">
        <h2>메모</h2>
        <span className={`small ${state === "offline" ? "warn" : "muted"}`}>{label[state]}</span>
      </div>
      <textarea
        value={text ?? ""}
        onChange={(e) => onChange(e.target.value)}
        onBlur={() => state === "dirty" && schedule(0)}
        placeholder="풀이 아이디어, 막힌 부분, 반례, 시간 복잡도... (problems/<번호>/memo.md 에 저장)"
        rows={compact ? 5 : 8}
        disabled={text === null}
      />
      {text === null && memo.isError && <div className="small warn">서버에 연결되면 메모를 불러옵니다.</div>}
      {memo.data && <div className="muted small mono">{memo.data.path}</div>}
    </section>
  );
}

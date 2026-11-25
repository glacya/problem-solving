// 여러 페이지에서 쓰는 작은 컴포넌트들.
import { useEffect, useRef, useState } from "react";
import { Link } from "react-router";
import renderMathInElement from "katex/contrib/auto-render";
import type { Problem, Statement, Status } from "./api";
import { bookmarklet } from "./bojSource";

const TIERS = ["Bronze", "Silver", "Gold", "Platinum", "Diamond", "Ruby"];
const ROMAN = ["V", "IV", "III", "II", "I"];

/** 푼 문제만 티어를 보여준다. level 이 null 이면 (서버가 가린 것) 아무것도 그리지 않는다. */
export function Tier({ level }: { level: number | null }) {
  if (level === null) return null;
  if (level === 0) return <span className="tier tier-unrated">Unrated</span>;
  const t = Math.floor((level - 1) / 5);
  return <span className={`tier tier-${t}`}>{`${TIERS[t]} ${ROMAN[(level - 1) % 5]}`}</span>;
}

export function Tags({ tags }: { tags: string[] | null }) {
  if (!tags?.length) return null;
  return (
    <span className="tags">
      {tags.map((t) => (
        <span key={t} className="tag">
          #{t}
        </span>
      ))}
    </span>
  );
}

export const STATUS_LABEL: Record<Status, string> = {
  queued: "대기",
  current: "풀이 중",
  paused: "풀다 만",
  later: "나중에",
  skipped: "스킵",
  solved: "AC",
};

export function StatusBadge({ status }: { status: Status }) {
  return <span className={`badge badge-${status}`}>{STATUS_LABEL[status]}</span>;
}

export function ProblemLink({ p }: { p: Pick<Problem, "id" | "title"> }) {
  return (
    <Link to={`/problems/${p.id}`} className="plink">
      <span className="pid">{p.id}</span> {p.title}
    </Link>
  );
}

export function CopyButton({ text, label = "복사" }: { text: string; label?: string }) {
  const [done, setDone] = useState(false);
  return (
    <button
      className="btn"
      onClick={async () => {
        await navigator.clipboard.writeText(text);
        setDone(true);
        setTimeout(() => setDone(false), 1500);
      }}
    >
      {done ? "복사됨 ✓" : label}
    </button>
  );
}

/** BOJ 지문 HTML (서버에서 정리된 것) + $...$ 수식 렌더링. */
export function StatementView({ st }: { st: Statement }) {
  const ref = useRef<HTMLDivElement>(null);
  useEffect(() => {
    if (ref.current) {
      renderMathInElement(ref.current, {
        delimiters: [
          { left: "$$", right: "$$", display: true },
          { left: "$", right: "$", display: false },
          { left: "\\(", right: "\\)", display: false },
          { left: "\\[", right: "\\]", display: true },
        ],
        throwOnError: false,
        ignoredTags: ["script", "noscript", "style", "textarea", "pre", "code"],
      });
    }
  }, [st]);
  return (
    <div ref={ref} className="statement">
      {st.sections.map((s) => (
        <section key={s.key}>
          <h2>{s.name}</h2>
          <div className="statement-body" dangerouslySetInnerHTML={{ __html: s.html }} />
        </section>
      ))}
      {st.samples.map((s, i) => (
        <section key={i} className="sample-row">
          <div>
            <div className="sample-head">
              <h3>예제 입력 {i}</h3>
              <CopyButton text={s.input} />
            </div>
            <pre className="sample">{s.input}</pre>
          </div>
          <div>
            <div className="sample-head">
              <h3>예제 출력 {i}</h3>
              <CopyButton text={s.output} />
            </div>
            <pre className="sample">{s.output}</pre>
          </div>
        </section>
      ))}
    </div>
  );
}

/** 북마크바로 끌어다 놓는 북마클릿 링크. React 는 javascript: 주소를 막으므로 DOM 에 직접 넣는다. */
export function BookmarkletLink() {
  const ref = useRef<HTMLAnchorElement>(null);
  useEffect(() => {
    ref.current?.setAttribute("href", bookmarklet(window.location.origin));
  }, []);
  return (
    <a
      ref={ref}
      className="bookmarklet"
      onClick={(e) => {
        e.preventDefault();
        alert("이 링크를 북마크바로 끌어다 놓은 뒤, BOJ 소스 페이지에서 그 북마크를 누르세요.");
      }}
    >
      📥 PS 소스 추가
    </a>
  );
}

export function ErrorBox({ error }: { error: unknown }) {
  if (!error) return null;
  return <div className="error">{error instanceof Error ? error.message : String(error)}</div>;
}

/** 버튼을 누르면 사유 입력칸이 열리고, 확인하면 onSubmit(사유). 스킵 / 나중에 풀기에 쓴다. */
export function ReasonButton({
  label,
  placeholder,
  onSubmit,
  busy,
  danger,
}: {
  label: string;
  placeholder: string;
  onSubmit: (reason: string) => void;
  busy?: boolean;
  danger?: boolean;
}) {
  const [open, setOpen] = useState(false);
  const [reason, setReason] = useState("");
  if (!open)
    return (
      <button className="btn" onClick={() => setOpen(true)}>
        {label}
      </button>
    );
  return (
    <form
      className="inline-form"
      onSubmit={(e) => {
        e.preventDefault();
        onSubmit(reason.trim());
        setOpen(false);
      }}
    >
      <input autoFocus value={reason} onChange={(e) => setReason(e.target.value)} placeholder={placeholder} />
      <button className={`btn ${danger ? "btn-danger" : "btn-primary"}`} disabled={busy}>
        {label}
      </button>
      <button type="button" className="btn btn-ghost" onClick={() => setOpen(false)}>
        취소
      </button>
    </form>
  );
}

export const SkipButton = (p: { onSubmit: (reason: string) => void; busy?: boolean }) => (
  <ReasonButton label="스킵" placeholder="스킵 사유 (선택)" danger {...p} />
);

export const LaterButton = (p: { onSubmit: (reason: string) => void; busy?: boolean }) => (
  <ReasonButton label="나중에 풀기" placeholder="왜 미루는지 (선택)" {...p} />
);

/** 푼 날짜 (YYYY-MM-DD, 시각 없음) → "2026-11-24". */
export function formatDay(s: string | null) {
  return s ? s.slice(0, 10) : "";
}

export function formatDate(s: string | null) {
  if (!s) return "";
  const d = new Date(s);
  return `${d.getMonth() + 1}/${d.getDate()} ${String(d.getHours()).padStart(2, "0")}:${String(d.getMinutes()).padStart(2, "0")}`;
}

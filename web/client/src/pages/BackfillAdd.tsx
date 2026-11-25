// 예전에 푼 문제의 소스를 직접 붙여넣어 solution/ 에 둔다 (커밋은 사용자가 직접 한다). 제목과 난이도는 서버가 solved.ac 에서 받는다.
import { useCallback, useEffect, useState } from "react";
import { Link, useSearchParams } from "react-router";
import { useMutation, useQuery, useQueryClient } from "@tanstack/react-query";
import { api, useMissing, useSettings, type BojPage, type Missing } from "../api";
import { BookmarkletLink, ErrorBox, Tags, Tier } from "../components";

const LAST_LANG = "ps.backfill.lang";

/** 서버의 gitops.solution_rel 과 같은 규칙: solution/boj/2000-2999/2300-2399/2345.cpp */
function solutionPath(id: number, ext: string) {
  const b = (size: number) => `${Math.floor(id / size) * size}-${Math.floor(id / size) * size + size - 1}`;
  return `solution/boj/${b(1000)}/${b(100)}/${id}.${ext}`;
}

/** 붙여넣은 코드로 언어를 짐작한다. 직접 고른 뒤에는 바꾸지 않는다. */
function guessLang(code: string): string | null {
  if (/^\s*#include\s*[<"]/m.test(code))
    return /\bstd::|using\s+namespace|iostream|bits\/stdc\+\+|template\s*<|\bvector\s*</.test(code) ? "cpp" : "c";
  if (/\bpublic\s+static\s+void\s+main\b/.test(code)) return "java";
  if (/^\s*fun\s+main\b/m.test(code)) return "kt";
  if (/^\s*fn\s+main\b/m.test(code)) return "rs";
  if (/^\s*package\s+main\b/m.test(code)) return "go";
  if (/^\s*using\s+System\b/m.test(code)) return "cs";
  if (/^\s*(import|from)\s+\w|^\s*def\s+\w+\(|\binput\(\)|\bprint\(/m.test(code)) return "py";
  return null;
}

function today() {
  const d = new Date();
  return `${d.getFullYear()}-${String(d.getMonth() + 1).padStart(2, "0")}-${String(d.getDate()).padStart(2, "0")}`;
}

export default function BackfillAdd() {
  const [params, setParams] = useSearchParams();
  const qc = useQueryClient();
  const settings = useSettings();

  const [idText, setIdText] = useState(params.get("id") ?? "");
  const [id, setId] = useState<number | null>(null); // 입력이 멈춘 뒤의 번호 (그때 solved.ac 에서 정보를 받는다)
  const [date, setDate] = useState("");
  const [lang, setLang] = useState(() => localStorage.getItem(LAST_LANG) ?? "cpp");
  const [langPicked, setLangPicked] = useState(false);
  const [code, setCode] = useState("");
  const [page, setPage] = useState<BojPage | null>(null); // 붙여넣은 BOJ 소스 페이지
  const [pageError, setPageError] = useState("");

  // 주소의 ?id= 가 바뀌면 (목록에서 다른 문제를 누르거나 '다음 문제') 입력칸도 따라간다.
  useEffect(() => setIdText(params.get("id") ?? ""), [params]);
  useEffect(() => {
    const t = setTimeout(() => setId(/^\d{4,6}$/.test(idText) ? +idText : null), 400);
    return () => clearTimeout(t);
  }, [idText]);

  /** BOJ 소스 페이지 HTML 을 해석해 번호, 날짜, 언어, 코드를 채운다. */
  const applyPage = useCallback(
    async (html: string) => {
      let s: BojPage;
      try {
        s = await api.parsePage(html);
      } catch {
        setPage(null);
        setPageError(
          "BOJ 소스 페이지로 읽을 수 없습니다. 소스 페이지에서 Ctrl+U (페이지 소스 보기) → Ctrl+A → Ctrl+C 한 내용을 붙여넣으세요.",
        );
        return;
      }
      save.reset();
      setPageError("");
      setPage(s);
      setIdText(String(s.problemId));
      setId(s.problemId);
      if (s.date) setDate(s.date);
      if (s.ext) {
        setLang(s.ext);
        setLangPicked(true);
      }
      setCode(s.code);
    },
    // eslint-disable-next-line react-hooks/exhaustive-deps
    [],
  );

  // 입력칸 밖에서 Ctrl+V 해도 페이지로 받는다.
  useEffect(() => {
    const onPaste = (e: ClipboardEvent) => {
      const t = e.target as HTMLElement | null;
      if (t && (t.tagName === "INPUT" || t.tagName === "TEXTAREA" || t.isContentEditable)) return;
      const text = e.clipboardData?.getData("text/plain") ?? "";
      if (!text) return;
      e.preventDefault();
      applyPage(text);
    };
    document.addEventListener("paste", onPaste);
    return () => document.removeEventListener("paste", onPaste);
  }, [applyPage]);

  const info = useQuery({
    queryKey: ["backfill-info", id],
    queryFn: () => api.backfillInfo(id!),
    enabled: id !== null,
    retry: false,
    staleTime: 10 * 60_000,
  });

  const save = useMutation({
    mutationFn: () => api.backfill(id!, date, lang, code),
    onSuccess: (r) => {
      localStorage.setItem(LAST_LANG, lang);
      // 목록을 다시 받지 않고 캐시에서만 뺀다.
      qc.setQueryData<Missing>(["backfill-missing"], (m) =>
        m && { ...m, local: m.local + 1, problems: m.problems.filter((p) => p.id !== r.id) },
      );
      qc.invalidateQueries({ queryKey: ["backfill-info", r.id] });
      qc.invalidateQueries({ queryKey: ["problems"] });
    },
  });

  // '다음 문제': 소스가 없는 문제 중 지금 번호 다음 것.
  const missing = useMissing();
  const rows = missing.data?.problems ?? [];
  const next = rows.find((p) => p.id > (id ?? 0) && p.id !== save.data?.id) ?? rows.find((p) => p.id !== id);

  const goNext = (nid: number) => {
    save.reset();
    setCode("");
    setLangPicked(false);
    setPage(null);
    setPageError("");
    setParams({ id: String(nid) });
  };

  const i = info.data;
  const exists = (i?.files.length ?? 0) > 0;
  const canSave = id !== null && !!i && !exists && /^\d{4}-\d{2}-\d{2}$/.test(date) && code.trim() !== "" && !save.isPending;

  return (
    <div className="stack">
      <section className="card">
        <div className="card-head">
          <h2>소스 추가</h2>
          <Link to="/backfill" className="btn btn-ghost">
            ← 목록
          </Link>
          {next && (
            <button className={`btn ${save.isSuccess ? "btn-primary" : ""}`} onClick={() => goNext(next.id)}>
              다음: {next.id} {next.title} →
            </button>
          )}
        </div>

        <div
          className="paste-zone"
          onDragOver={(e) => e.preventDefault()}
          onDrop={(e) => {
            e.preventDefault();
            const f = e.dataTransfer.files[0];
            if (f) f.text().then(applyPage);
            else applyPage(e.dataTransfer.getData("text/plain"));
          }}
        >
          <textarea
            className="paste-input"
            rows={2}
            value=""
            readOnly
            onPaste={(e) => {
              e.preventDefault();
              applyPage(e.clipboardData.getData("text/plain"));
            }}
            placeholder="BOJ 소스 페이지 붙여넣기: 소스 페이지에서 Ctrl+U → Ctrl+A → Ctrl+C 한 뒤 여기서 Ctrl+V (저장한 .html 파일을 끌어 놓아도 됩니다)"
          />
          {page && (
            <div className="small">
              <b>
                제출 {page.submissionId ?? "?"}번 · {page.problemId} {page.problemTitle} · {page.language} · {page.date}
              </b>{" "}
              을 불러왔습니다.
              {page.warnings.map((w) => (
                <div key={w} className="warn">
                  {w}
                </div>
              ))}
              {params.get("id") && params.get("id") !== String(page.problemId) && (
                <div className="warn">목록에서 고른 {params.get("id")}번이 아니라 {page.problemId}번 페이지입니다.</div>
              )}
            </div>
          )}
          {pageError && <div className="warn small">{pageError}</div>}
          <div className="muted small">
            여러 문제를 넣을 때는 <BookmarkletLink /> 북마클릿이 편합니다 (사용법은 <Link to="/backfill">목록</Link> 위에 있습니다).
          </div>
        </div>

        <form
          className="stack"
          onSubmit={(e) => {
            e.preventDefault();
            if (canSave) save.mutate();
          }}
          onKeyDown={(e) => {
            if (e.key === "Enter" && (e.ctrlKey || e.metaKey) && canSave) {
              e.preventDefault();
              save.mutate();
            }
          }}
        >
          <div className="inline-form">
            <label className="field">
              번호
              <input
                value={idText}
                onChange={(e) => {
                  save.reset();
                  setIdText(e.target.value.replace(/\D/g, ""));
                }}
                placeholder="1000"
                style={{ width: 110 }}
              />
            </label>
            <label className="field">
              푼 날짜
              <input type="date" value={date} max={today()} onChange={(e) => setDate(e.target.value)} required />
            </label>
            <label className="field">
              언어
              <select
                value={lang}
                onChange={(e) => {
                  setLang(e.target.value);
                  setLangPicked(true);
                }}
              >
                {(settings.data?.sourceLangs ?? [{ id: "cpp", label: "C++" }]).map((l) => (
                  <option key={l.id} value={l.id}>
                    {l.label} (.{l.id})
                  </option>
                ))}
              </select>
            </label>
          </div>

          <div className="backfill-info">
            {id === null ? (
              <span className="muted small">번호를 넣으면 solved.ac 에서 제목과 난이도를 받아 옵니다.</span>
            ) : info.isPending ? (
              <span className="muted small">불러오는 중…</span>
            ) : i ? (
              <>
                <div className="meta-line">
                  <a href={`https://www.acmicpc.net/problem/${i.id}`} target="_blank" rel="noreferrer" className="plink">
                    <span className="pid">{i.id}</span> {i.title}
                  </a>
                  <Tier level={i.level} />
                  <Tags tags={i.tags} />
                  {i.statusUrl && (
                    <a href={i.statusUrl} target="_blank" rel="noreferrer" className="small">
                      BOJ 에서 내 맞은 제출 보기 ↗
                    </a>
                  )}
                </div>
                {!i.solved && (
                  <div className="warn small">
                    solved.ac 에 아직 푼 문제로 나오지 않습니다 (티어는 가립니다). BOJ 에서 AC 를 확인했다면 그대로 추가해도
                    됩니다.
                  </div>
                )}
                {exists && <div className="warn small">이미 소스가 있습니다: {i.files.join(", ")}</div>}
              </>
            ) : null}
            <ErrorBox error={info.error} />
          </div>

          <label className="field">
            코드 (BOJ 에 제출한 그대로)
            <textarea
              className="code-input"
              value={code}
              spellCheck={false}
              onChange={(e) => {
                setCode(e.target.value);
                if (!langPicked) {
                  const g = guessLang(e.target.value);
                  if (g && settings.data?.sourceLangs.some((l) => l.id === g)) setLang(g);
                }
              }}
              placeholder="여기에 소스를 붙여넣으세요"
              rows={20}
            />
          </label>

          <div className="actions">
            <button className="btn btn-primary" disabled={!canSave}>
              {save.isPending ? "저장하는 중…" : "저장 (Ctrl+Enter)"}
            </button>
            {id !== null && (
              <span className="muted small">
                <code>
                  {solutionPath(id, lang)}
                </code>
                {" "}에 저장합니다 (커밋하지 않음)
              </span>
            )}
          </div>
        </form>

        {save.data && (
          <div className="ok-box">
            <code>{save.data.path}</code> 를 만들고 <code>{save.data.history}</code> 를 다시 썼습니다. 커밋은 직접 하세요.
          </div>
        )}
        <ErrorBox error={save.error} />
      </section>
    </div>
  );
}

// 예전에 푼 문제의 소스를 직접 붙여넣어 solution/ 에 둔다 (커밋은 사용자가 직접 한다). 제목과 난이도는 서버가 solved.ac 에서 받는다.
import { useEffect, useState } from "react";
import { Link, useNavigate, useSearchParams } from "react-router";
import { useMutation, useQuery, useQueryClient } from "@tanstack/react-query";
import { api, useSettings, type Missing } from "../api";
import { ErrorBox, Tags, Tier } from "../components";

const LAST_LANG = "ps.backfill.lang";

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
  const navigate = useNavigate();
  const qc = useQueryClient();
  const settings = useSettings();

  const [idText, setIdText] = useState(params.get("id") ?? "");
  const [id, setId] = useState<number | null>(null); // 입력이 멈춘 뒤의 번호 (그때 solved.ac 에서 정보를 받는다)
  const [date, setDate] = useState("");
  const [lang, setLang] = useState(() => localStorage.getItem(LAST_LANG) ?? "cpp");
  const [langPicked, setLangPicked] = useState(false);
  const [code, setCode] = useState("");

  // 주소의 ?id= 가 바뀌면 (목록에서 다른 문제를 누르거나 '다음 문제') 입력칸도 따라간다.
  useEffect(() => setIdText(params.get("id") ?? ""), [params]);
  useEffect(() => {
    const t = setTimeout(() => setId(/^\d{4,6}$/.test(idText) ? +idText : null), 400);
    return () => clearTimeout(t);
  }, [idText]);

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

  // 저장한 뒤 '다음 문제': 목록 캐시에서 지금 번호 다음 것 (목록을 열어 본 적이 있을 때만).
  const missing = qc.getQueryData<Missing>(["backfill-missing"]);
  const next = save.isSuccess
    ? (missing?.problems.find((p) => p.id > (save.data?.id ?? 0)) ?? missing?.problems[0])
    : undefined;

  const goNext = (nid: number) => {
    save.reset();
    setCode("");
    setLangPicked(false);
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
        </div>

        <form
          className="stack"
          onSubmit={(e) => {
            e.preventDefault();
            if (canSave) save.mutate();
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
              {save.isPending ? "저장하는 중…" : "저장"}
            </button>
            {id !== null && (
              <span className="muted small">
                <code>
                  solution/boj/{id}.{lang}
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
        {save.isSuccess && (
          <div className="actions">
            {next ? (
              <button className="btn btn-primary" onClick={() => goNext(next.id)}>
                다음 문제: {next.id} {next.title}
              </button>
            ) : null}
            <button className="btn" onClick={() => navigate("/backfill")}>
              목록으로
            </button>
          </div>
        )}
        <ErrorBox error={save.error} />
      </section>
    </div>
  );
}

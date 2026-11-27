import { useEffect, useState } from "react";
import { Link, useParams } from "react-router";
import { useMutation, useQuery } from "@tanstack/react-query";
import {
  api,
  useDetail,
  useProblemMutation,
  useSettings,
  type JudgeResult,
  type Lang,
  type Problem,
  type TestResult,
} from "../api";
import { CopyButton, ErrorBox, LaterButton, StatusBadge, Tags, Tier, formatDate, formatDay } from "../components";
import MemoEditor from "../Memo";

export default function SubmitPage() {
  const id = Number(useParams().id);
  const detail = useDetail(id);
  const settings = useSettings();
  const [lang, setLang] = useState<Lang | null>(null);
  const later = useProblemMutation((reason: string) => api.setStatus(id, "later", reason));

  useEffect(() => {
    if (detail.data && lang === null) {
      const { problem, langs } = detail.data;
      setLang(langs.includes(problem.language) ? problem.language : (langs[0] ?? problem.language));
    }
  }, [detail.data, lang]);

  if (detail.isPending || lang === null) return <p className="muted">불러오는 중...</p>;
  if (detail.error) return <ErrorBox error={detail.error} />;
  const p = detail.data.problem;
  const langs = detail.data.langs;

  return (
    <div className="stack">
      <section className="card">
        <div className="muted small">제출 준비</div>
        <h1>
          <Link to={`/problems/${id}`} className="plink">
            <span className="pid">{p.id}</span> {p.title}
          </Link>
        </h1>
        <div className="meta-line">
          <StatusBadge status={p.status} />
          <Tier level={p.level} />
          <Tags tags={p.tags} />
          <span className="muted small">
            {p.time_limit} · {p.memory_limit}
          </span>
          <span className="spacer" />
          {["current", "paused", "queued"].includes(p.status) && (
            <LaterButton busy={later.isPending} onSubmit={(r) => later.mutate(r)} />
          )}
        </div>
        {langs.length > 1 && (
          <div className="tabs">
            {(settings.data?.langs ?? []).filter((l) => langs.includes(l.id)).map((l) => (
              <button key={l.id} className={`tab ${lang === l.id ? "on" : ""}`} onClick={() => setLang(l.id)}>
                {l.label}
              </button>
            ))}
          </div>
        )}
        <ErrorBox error={later.error} />
      </section>
      <div className="with-aside">
        <div className="stack">
          {langs.length === 0 ? (
            <section className="card">
              <p className="muted">
                아직 풀이 파일이 없습니다. <Link to={`/problems/${id}`}>문제 페이지</Link>에서 언어를 골라 시작하세요.
              </p>
            </section>
          ) : (
            <Steps p={p} lang={lang} bojLang={settings.data?.langs.find((l) => l.id === lang)?.boj ?? lang} />
          )}
        </div>
        <aside className="aside">
          <MemoEditor id={id} compact />
        </aside>
      </div>
    </div>
  );
}

function Steps({ p, lang, bojLang }: { p: Problem; lang: Lang; bojLang: string }) {
  const id = p.id;
  // VSCode 에서 고치고 돌아오면(창 포커스) 번들을 다시 만든다.
  const bundle = useQuery({ queryKey: ["bundle", id, lang], queryFn: () => api.bundle(id, lang), refetchOnWindowFocus: true });
  const judge = useMutation({ mutationFn: () => api.judge(id, lang) });
  const submit = useProblemMutation(() => api.submit(id, lang));
  const check = useProblemMutation(() => api.check(id));
  const markSolved = useProblemMutation(() => api.markSolved(id, lang));
  const watch = useProblemMutation(() => api.watch(id));
  const unwatch = useProblemMutation(() => api.unwatch(id));
  // 서버가 AC 를 확인하는 동안 화면의 상태를 10초마다 다시 불러온다.
  // (확인 자체는 서버가 한다: 탭을 닫아도, 서버를 재시작해도 이어진다.) 같은 키라 위의 useDetail 캐시를 갱신한다.
  useQuery({ queryKey: ["problem", id], queryFn: () => api.detail(id), refetchInterval: p.watching ? 10_000 : false });

  const allPass = judge.data && judge.data.compile.ok && judge.data.results.every((r) => r.status === "PASS" || r.status === "SKIP");

  return (
    <>
      <section className="card">
        <div className="card-head">
          <h2>
            <span className="step">1</span> 테스트
          </h2>
          <button className="btn btn-primary" onClick={() => judge.mutate()} disabled={judge.isPending}>
            {judge.isPending ? "실행 중..." : "테스트 실행"}
          </button>
        </div>
        <p className="muted small">
          제출할 번들 그대로 컴파일해 <code>inputs/</code> 의 모든 입력을 돌립니다. 줄 끝 공백 1개와 파일 끝 빈 줄 1개는 허용합니다.
        </p>
        {judge.data && <JudgeView r={judge.data} />}
        <ErrorBox error={judge.error} />
      </section>

      <section className="card">
        <div className="card-head">
          <h2>
            <span className="step">2</span> 제출 코드
          </h2>
          <span className="spacer" />
          <button className="btn btn-ghost" onClick={() => bundle.refetch()} disabled={bundle.isFetching}>
            {bundle.isFetching ? "만드는 중..." : "다시 만들기"}
          </button>
          {bundle.data && <CopyButton text={bundle.data.code} label="코드 복사" />}
        </div>
        {bundle.data && (
          <>
            <p className="muted small">
              {bundle.data.lines}줄 · {bundle.data.bytes.toLocaleString()} bytes
              {bundle.data.used.length > 0 && <> · 포함한 파일: {bundle.data.used.map((u) => u.split("/").pop()).join(", ")}</>}
            </p>
            <pre className="code">{bundle.data.code}</pre>
          </>
        )}
        <ErrorBox error={bundle.error} />
      </section>

      <section className="card">
        <div className="card-head">
          <h2>
            <span className="step">3</span> BOJ 에 제출
          </h2>
        </div>
        <p className="small">
          지금의 번들을 제출본으로 저장하고 클립보드에 복사한 뒤 BOJ 제출 페이지를 엽니다. 언어는 <b>{bojLang}</b>를 고르세요.
          {judge.data && !allPass && <span className="warn"> 테스트를 통과하지 못했습니다.</span>}
        </p>
        <button
          className="btn btn-primary"
          disabled={submit.isPending}
          onClick={() =>
            submit.mutate(undefined, {
              onSuccess: async (r) => {
                await navigator.clipboard.writeText(r.code).catch(() => {});
                window.open(r.submitUrl, "_blank", "noreferrer");
              },
            })
          }
        >
          제출본 저장 · 복사 · 제출 페이지 열기 ↗
        </button>
        {submit.data && (
          <p className="muted small">제출본을 저장했습니다 ({submit.data.lines}줄). 서버가 AC 를 확인하면 이 파일을 커밋합니다.</p>
        )}
        <ErrorBox error={submit.error} />
      </section>

      <section className="card">
        <div className="card-head">
          <h2>
            <span className="step">4</span> AC 확인 · 커밋
          </h2>
        </div>
        <p className="muted small">
          제출본을 저장하면 서버가 15분 동안 30초마다 solved.ac 를 확인하고, AC 가 보이면 제출본을 <code>solution/boj/</code> 에
          커밋합니다 (push 는 하지 않습니다). 제출본 없이 BOJ 에 바로 제출해도, 풀이 파일을 고친 뒤 3시간 동안은 2분마다 확인하고 지금 코드의
          번들을 커밋합니다. 브라우저를 닫거나 서버를 재시작해도 이어서 확인합니다. 이전에 푼 문제는 바로 AC 로 보입니다.
        </p>
        {p.status === "solved" ? (
          <div className="ok-box">
            <b>AC</b> {formatDay(p.solved_at)} <Tier level={p.level} /> <Tags tags={p.tags} />
            {p.commit_log && <pre className="log">{p.commit_log}</pre>}
            {p.commit_log.startsWith("실패") && (
              <div className="actions">
                <button className="btn" onClick={() => check.mutate()} disabled={check.isPending}>
                  {check.isPending ? "커밋하는 중…" : "다시 커밋"}
                </button>
                <span className="muted small">제출본이 없으면 지금 코드의 번들을 커밋합니다.</span>
              </div>
            )}
          </div>
        ) : (
          <div className="actions">
            {p.watching ? (
              <>
                <span className="small warn">● 서버가 AC 를 확인하는 중 ({formatDate(p.watch_until)} 까지)</span>
                <button className="btn btn-ghost" onClick={() => unwatch.mutate()}>
                  그만 확인
                </button>
              </>
            ) : (
              <button className="btn btn-ghost" onClick={() => watch.mutate()}>
                15분 동안 확인
              </button>
            )}
            <button className="btn" onClick={() => check.mutate()} disabled={check.isPending}>
              지금 확인
            </button>
            <span className="spacer" />
            <button
              className="btn"
              disabled={markSolved.isPending}
              title="solved.ac 확인 없이 AC 로 기록하고 커밋합니다"
              onClick={() => {
                const what = p.submitted_at ? "저장한 제출본" : "지금 코드의 번들 (제출본을 저장하지 않았습니다)";
                if (confirm(`BOJ 에서 AC 를 확인했나요?\n${what}을 solution/boj/ 에 커밋합니다.`))
                  markSolved.mutate();
              }}
            >
              직접 AC 처리
            </button>
          </div>
        )}
        {check.data && !check.data.solved && <p className="muted small">{check.data.message}</p>}
        <ErrorBox error={check.error ?? watch.error ?? unwatch.error ?? markSolved.error} />
      </section>
    </>
  );
}

function JudgeView({ r }: { r: JudgeResult }) {
  if (!r.compile.ok)
    return (
      <div className="error">
        <b>컴파일 에러</b>
        <pre className="log">{r.compile.log}</pre>
      </div>
    );
  const pass = r.results.filter((x) => x.status === "PASS").length;
  const skip = r.results.filter((x) => x.status === "SKIP").length;
  const fail = r.results.length - pass - skip;
  return (
    <>
      <p className={`summary ${fail ? "bad" : "good"}`}>
        {fail ? `${fail} failed, ` : ""}
        {pass} passed{skip ? `, ${skip} skipped` : ""} · 제한 {r.limitMs} ms
      </p>
      <ul className="results">
        {r.results.map((t) => (
          <ResultRow key={t.name} t={t} limitMs={r.limitMs} />
        ))}
      </ul>
    </>
  );
}

function ResultRow({ t, limitMs }: { t: TestResult; limitMs: number }) {
  const failed = t.status !== "PASS" && t.status !== "SKIP";
  const [open, setOpen] = useState(failed);
  return (
    <li className={`result result-${t.status}`}>
      <button className="result-head" onClick={() => setOpen(!open)}>
        <span className="result-status">{t.status}</span>
        <span className="result-name">{t.name}</span>
        <span className="muted small">{t.reason || (t.status === "SKIP" ? "정답 없음 → answers/" + t.name + ".out" : "")}</span>
        <span className="spacer" />
        <span className={`small ${t.ms > limitMs ? "warn" : "muted"}`}>{t.ms} ms</span>
        <span className="muted small">{Math.round(t.memKb / 1024)} MB</span>
      </button>
      {open && (
        <div className="io">
          <div>
            <h4>입력</h4>
            <pre>{t.input}</pre>
          </div>
          {t.expected !== null && (
            <div>
              <h4>정답</h4>
              <pre>{t.expected}</pre>
            </div>
          )}
          <div>
            <h4>출력</h4>
            <pre>{t.output}</pre>
          </div>
          {t.stderr && (
            <div>
              <h4>stderr</h4>
              <pre>{t.stderr}</pre>
            </div>
          )}
        </div>
      )}
    </li>
  );
}

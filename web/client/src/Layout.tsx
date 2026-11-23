import { NavLink, Outlet } from "react-router";
import { useHealth, useSettings } from "./api";

export default function Layout() {
  const settings = useSettings();
  const health = useHealth();
  return (
    <>
      <header className="topbar">
        <nav className="topbar-inner">
          <NavLink to="/" className="brand" end>
            PS
          </NavLink>
          <NavLink to="/" end>
            문제
          </NavLink>
          <NavLink to="/history">기록</NavLink>
          <span className="spacer" />
          {settings.data && (
            <span className="muted small">
              {settings.data.handle || "BOJ_HANDLE 미설정"}
              {!settings.data.hasKey && " · solved.ac 키 없음"}
            </span>
          )}
        </nav>
      </header>
      {health.isError && (
        <div className="offline-banner">
          서버에 연결할 수 없습니다. 터미널에서 <code>./ps.sh</code> 로 다시 띄우세요. 메모는 브라우저에 임시 저장되고, 연결되면
          자동으로 저장됩니다.
        </div>
      )}
      <main className="page">
        <Outlet />
      </main>
    </>
  );
}

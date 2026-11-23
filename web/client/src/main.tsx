import { StrictMode } from "react";
import { createRoot } from "react-dom/client";
import { QueryClient, QueryClientProvider } from "@tanstack/react-query";
import { createBrowserRouter, RouterProvider } from "react-router";
import "katex/dist/katex.min.css";
import "./styles.css";

import Layout from "./Layout";
import Home from "./pages/Home";
import ProblemPage from "./pages/ProblemPage";
import SubmitPage from "./pages/SubmitPage";
import History from "./pages/History";

const queryClient = new QueryClient({
  defaultOptions: { queries: { refetchOnWindowFocus: true, retry: 1 } },
});

// 기능이 늘면 여기에 라우트를 더한다 (예: /contest, /notes ...).
const router = createBrowserRouter([
  {
    path: "/",
    element: <Layout />,
    children: [
      { index: true, element: <Home /> },
      { path: "problems/:id", element: <ProblemPage /> },
      { path: "problems/:id/submit", element: <SubmitPage /> },
      { path: "history", element: <History /> },
    ],
  },
]);

createRoot(document.getElementById("root")!).render(
  <StrictMode>
    <QueryClientProvider client={queryClient}>
      <RouterProvider router={router} />
    </QueryClientProvider>
  </StrictMode>,
);

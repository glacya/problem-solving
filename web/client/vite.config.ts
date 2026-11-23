import { defineConfig } from "vite";
import react from "@vitejs/plugin-react";

// 개발 중에는 `npm run dev` (5173) 가 /api 를 백엔드(ps.sh, 8710)로 넘긴다.
export default defineConfig({
  plugins: [react()],
  server: {
    host: "127.0.0.1",
    proxy: { "/api": `http://127.0.0.1:${process.env.PS_PORT ?? 8710}` },
  },
});

import { defineConfig } from "vite";
import { resolve, dirname } from "path";
import { fileURLToPath } from "url";
import react from "@vitejs/plugin-react";
import typescript from "@rollup/plugin-typescript";
import cssInjectedByJsPlugin from "vite-plugin-css-injected-by-js";

const __dirname = dirname(fileURLToPath(import.meta.url));

export default defineConfig({
  define: { "process.env.NODE_ENV": "'production'" },
  plugins: [react(), typescript(), cssInjectedByJsPlugin()],
  build: {
    lib: {
      entry: resolve(__dirname, "src/index.tsx"),
      name: "index",
      fileName: () => `parameters-mfe.js`,
      formats: ["umd"],
    },
  },
});

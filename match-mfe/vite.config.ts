import { configDefaults, defineConfig } from "vitest/config";

export default defineConfig({
  build: {
    rollupOptions: {
      input: {
        "match-mfe": "./index.html",
      },
      output: {
        entryFileNames: "[name].js",
      },
    },
  },
  worker: {
    format: "es",
    rollupOptions: {
      output: {
        entryFileNames: "[name].js",
      },
    },
  },
  test: {
    environment: "jsdom",
    coverage: {
      reporter: ["text", "json-summary", "json"],
      exclude: [...configDefaults.coverage.exclude!, "playwright.config.ts"],
    },
    exclude: [...configDefaults.exclude, "e2e/*"],
  },
  preview: {
    port: 3031,
    strictPort: true,
  },
  server: {
    port: 3031,
    strictPort: true,
    host: true,
  },
});

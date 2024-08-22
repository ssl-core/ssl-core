import { configDefaults, defineConfig } from "vitest/config";

export default defineConfig({
  build: {
    rollupOptions: {
      input: {
        "viewer-mfe": "./index.html",
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
    port: 3032,
    strictPort: true,
    host: "0.0.0.0",
  },
  server: {
    port: 3032,
    strictPort: true,
    host: "0.0.0.0",
  },
});

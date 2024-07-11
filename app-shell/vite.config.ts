import { configDefaults } from "vitest/config";
import ViteYaml from "@modyfi/vite-plugin-yaml";

export default {
  base: "/",
  plugins: [ViteYaml()],
  test: {
    environment: "jsdom",
    coverage: {
      reporter: ["text", "json-summary", "json"],
      exclude: [...configDefaults.coverage.exclude!, "playwright.config.ts"],
    },
    exclude: [...configDefaults.exclude, "e2e/*"],
  },
  preview: {
    port: 3030,
    strictPort: true,
  },
  server: {
    port: 3030,
    strictPort: true,
    host: true,
    origin: "http://0.0.0.0:3030",
  },
};

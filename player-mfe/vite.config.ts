import { defineConfig } from "vitest/config";
import { viteAwesomeSvgLoader } from "vite-awesome-svg-loader";

export default defineConfig({
  plugins: [viteAwesomeSvgLoader()],
  test: {
    environment: "happy-dom",
    coverage: {
      reporter: ["text", "json-summary", "json"],
    },
  },
  build: {
    rollupOptions: {
      input: {
        "player-mfe": "./index.html",
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
  preview: {
    port: 3031,
    strictPort: true,
    host: "0.0.0.0",
  },
  server: {
    port: 3031,
    strictPort: true,
    host: "0.0.0.0",
  },
});

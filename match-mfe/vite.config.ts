import { defineConfig } from "vite";

export default defineConfig({
  build: {
    rollupOptions: {
      input: {
        "match-mfe": "./index.html",
        "three-worker": "./src/app/engines/three/worker/three-worker.ts",
      },
      output: {
        entryFileNames: "[name].js",
      },
    },
  },
  preview: {
    port: 4001,
    strictPort: true,
  },
  server: {
    port: 4001,
    strictPort: true,
    host: true,
  },
});

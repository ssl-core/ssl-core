import { defineConfig } from 'vite'
import react from '@vitejs/plugin-react'

// https://vite.dev/config/
export default defineConfig({
  plugins: [react()],
  build: {
    rollupOptions: {
      input: {
        "params-mfe": "./index.html",
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
    port: 3033,
    strictPort: true,
    host: "0.0.0.0",
  },
  server: {
    port: 3033,
    strictPort: true,
    host: "0.0.0.0",
  },
})

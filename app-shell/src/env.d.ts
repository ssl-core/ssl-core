/// <reference types="@modyfi/vite-plugin-yaml/modules" />
/// <reference types="vite/client" />

type Route = {
  path: string;
  label: string;
  icon: string;
  rows: number;
  cols: number;
  components: {
    url: string;
    type: "webcomponent" | "iframe";
    metadata: Record<string, any>;
    rows: {
      start: number;
      end: number;
    };
    cols: {
      start: number;
      end: number;
    };
  }[];
};

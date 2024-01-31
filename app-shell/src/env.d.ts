/// <reference types="@modyfi/vite-plugin-yaml/modules" />
/// <reference types="vite/client" />

type RouteFragment = {
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
};

type Route = {
  path: string;
  label: string;
  icon: string;
  rows: number;
  cols: number;
  fragments: RouteFragment[];
};

type Shortcut = {
  eventName: string;
  data: Record<string, any>;
};

type Template = string;

import { afterEach, beforeEach, describe, expect, it, vi } from "vitest";

import Router from "../../../src/app/lib/router";
import indexHtml from "../../../index.html?raw";

describe("Router", () => {
  let routes: Route[] = [
    {
      path: "/another",
      label: "Another",
      icon: "heart",
      cols: 2,
      rows: 2,
      fragments: {
        first: {
          url: "https://first.example",
          type: "iframe",
          metadata: {},
          rows: { start: 1, end: 3 },
          cols: { start: 1, end: 2 },
        },
        second: {
          url: "https://second.example",
          type: "webcomponent",
          metadata: {
            tag: "second-fragment",
          },
          rows: { start: 1, end: 3 },
          cols: { start: 2, end: 3 },
        },
      },
    },
  ];
  let templates: Record<string, Template> = {
    "/": '<h1 id="root-template">Root</h1>',
    "/settings": '<h1 id="settings-template">Settings</h1>',
  };
  let router: Router;

  beforeEach(() => {
    document.body.innerHTML = indexHtml;
    router = new Router(routes, templates);
  });

  afterEach(() => {
    vi.unstubAllGlobals();
  });

  it("should render root page", () => {
    router.initialize();
    expect(document.querySelector("#root-template")).not.toBeNull();
  });

  it("should render /settings page", () => {
    vi.stubGlobal("location", { href: "https://www.example.com/settings" });

    router.initialize();
    expect(document.querySelector("#settings-template")).not.toBeNull();
  });

  it("should render /another page", () => {
    vi.stubGlobal("location", { href: "https://www.example.com/another" });

    router.initialize();
    expect(document.querySelector("iframe")).not.toBeNull();
    expect(
      document.querySelector('script[src="https://second.example"]')
    ).not.toBeNull();
  });

  it("should redirect to root page when not found", () => {
    vi.stubGlobal("location", { href: "https://www.example.com/unknown" });

    router.initialize();
    expect(document.querySelector("#root-template")).not.toBeNull();
  });

  it("should create nav items", () => {
    router.initialize();

    const navLink = document.querySelector('[href="/another"]')!;

    expect(navLink).not.toBeNull();
    expect(navLink.querySelector("i")?.dataset.lucide).toBe("heart");
    expect(navLink.querySelector("span")?.innerHTML).toBe("Another");
  });
});

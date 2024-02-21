import { beforeAll, describe, expect, it } from "vitest";

import WebComponentFragment from "../../../src/app/fragments/web-component-fragment";

describe("WebComponentFragment", () => {
  let webComponentFragment: WebComponentFragment;
  let url: string = "https://example.com";
  let container: HTMLElement = document.createElement("div");
  let metadata: Record<string, any> = { tag: "example-fragment" };

  beforeAll(() => {
    webComponentFragment = new WebComponentFragment(url, container, metadata);
  });

  it("should append script", () => {
    webComponentFragment.render();
    const script = document.body.querySelector(
      'script[src="https://example.com"]'
    );
    expect(script).not.toBeNull();
  });

  it("should create element", () => {
    webComponentFragment.render();

    const script = document.body.querySelector(
      'script[src="https://example.com"]'
    )!;
    script.dispatchEvent(new Event("load"));

    const element = container.querySelector("example-fragment");
    expect(element).not.toBeNull();
  });
});

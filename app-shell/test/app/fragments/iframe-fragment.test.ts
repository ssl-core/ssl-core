import { beforeAll, describe, expect, it } from "vitest";

import IframeFragment from "../../../src/app/fragments/iframe-fragment";

describe("IframeFragment", () => {
  let iframeFragment: IframeFragment;
  let url: string = "https://example.com";
  let container: HTMLElement = document.createElement("div");
  let metadata: Record<string, any> = {};

  beforeAll(() => {
    iframeFragment = new IframeFragment(url, container, metadata);
  });

  it("should create an iframe", () => {
    iframeFragment.render();
    const iframe = container.querySelector("iframe");
    expect(iframe).not.toBeNull();
  });
});

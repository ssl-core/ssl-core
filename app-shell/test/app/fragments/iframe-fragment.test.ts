import { beforeAll, describe, expect, it } from "vitest";

import IFrameFragment from "../../../src/app/fragments/iframe-fragment";

describe("IFrameFragment", () => {
  let iframeFragment: IFrameFragment;
  let url: string = "https://example.com";
  let container: HTMLElement = document.createElement("div");
  let metadata: Record<string, any> = {};

  beforeAll(() => {
    iframeFragment = new IFrameFragment(url, container, metadata);
  });

  it("should create an iframe", () => {
    iframeFragment.render();
    const iframe = container.querySelector("iframe");
    expect(iframe).not.toBeNull();
  });
});

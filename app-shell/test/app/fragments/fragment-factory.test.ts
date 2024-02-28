import { beforeAll, describe, expect, it } from "vitest";

import FragmentFactory from "../../../src/app/fragments/fragment-factory";

describe("FragmentFactory", () => {
  let fragmentFactory: FragmentFactory;
  let container: HTMLElement;

  beforeAll(() => {
    fragmentFactory = new FragmentFactory();
    container = document.createElement("div");
  });

  it("should create an iframe fragment", () => {
    const routeFragment: RouteFragment = {
      url: "https://example.com",
      type: "iframe",
      metadata: {},
      rows: { start: 1, end: 3 },
      cols: { start: 1, end: 2 },
    };
    const fragment = fragmentFactory.createFragment(container, routeFragment);

    expect(fragment.constructor.name).toBe("IFrameFragment");
  });

  it("should create a web component fragment", () => {
    const routeFragment: RouteFragment = {
      url: "https://example.com",
      type: "webcomponent",
      metadata: {
        tag: "example-fragment",
      },
      rows: { start: 1, end: 3 },
      cols: { start: 1, end: 2 },
    };
    const fragment = fragmentFactory.createFragment(container, routeFragment);

    expect(fragment.constructor.name).toBe("WebComponentFragment");
  });

  it("should throw an error for unknown fragment type", () => {
    expect(() => {
      const routeFragment = {
        url: "https://example.com",
        type: "unknown",
        metadata: {},
        rows: { start: 1, end: 3 },
        cols: { start: 1, end: 2 },
      };

      // @ts-expect-error
      fragmentFactory.createFragment(container, routeFragment);
    }).toThrowError("Unknown fragment type: unknown");
  });
});

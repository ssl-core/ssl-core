import { afterEach, describe, expect, it, vi } from "vitest";

describe("Main", () => {
  afterEach(() => {
    vi.unstubAllGlobals();
  });

  it("should define the webcomponent", async () => {
    const defineMock = vi.fn();
    vi.stubGlobal("customElements", { define: defineMock });

    await import("../src/main");

    expect(defineMock).toHaveBeenCalledWith("match-mfe", expect.any(Function));
  });
});

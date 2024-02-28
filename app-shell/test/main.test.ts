import { describe, expect, it, vi } from "vitest";

describe("Main", () => {
  it("should initialize the app", async () => {
    const initialize = vi.fn();
    const app = vi.fn().mockImplementation(() => ({
      initialize,
    }));

    vi.doMock("../src/app/app", () => ({ default: app }));

    await import("../src/main");

    expect(initialize).toHaveBeenCalled();
  });
});

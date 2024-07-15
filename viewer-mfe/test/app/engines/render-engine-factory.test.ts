import { describe, expect, it } from "vitest";

import RenderEngineFactory from "../../../src/app/engines/render-engine-factory";

describe("RenderEngineFactory", () => {
  it("should define class", () => {
    expect(RenderEngineFactory).toBeDefined();
  });
});

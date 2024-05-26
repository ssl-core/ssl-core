import { describe, expect, it } from "vitest";

import "../../../src/app/components/player-mfe-controls";

describe("PlayerMFEControls", () => {
  it("should render", () => {
    const element = document.createElement("player-mfe-controls");
    document.body.appendChild(element);
    expect(element.innerHTML).toContain("controls");
  });
});

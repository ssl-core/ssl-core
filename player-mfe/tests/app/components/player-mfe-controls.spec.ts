import { beforeAll, beforeEach, describe, expect, it, vi } from "vitest";

import PlayerMFEControls from "../../../src/app/components/player-mfe-controls";
import { provide } from "../../../src/app/services/global-provider";
import Playback from "../../../src/entities/playback";

describe("PlayerMFEControls", () => {
  const socketHandlerMock = {
    playLiveStream: vi.fn(),
    pauseLiveStream: vi.fn(),
    addEventListener: vi.fn(),
    fetchReplay: vi.fn(),
  } as any;
  const playback = new Playback(socketHandlerMock);

  let element: PlayerMFEControls;

  beforeAll(() => {
    if (!customElements.get("player-mfe-controls")) {
      customElements.define("player-mfe-controls", PlayerMFEControls);
    }

    provide("playback", playback);
  });

  beforeEach(() => {
    element = document.createElement(
      "player-mfe-controls"
    ) as PlayerMFEControls;
    document.body.appendChild(element);
  });

  it("should be defined", () => {
    expect(customElements.get("player-mfe-controls")).toBeDefined();
  });

  it("should render play button", () => {
    const playButton = element.querySelector("[data-testid=play-button]");
    expect(playButton).not.toBeNull();
  });

  it("should render rewind button", () => {
    const rewindButton = element.querySelector("[data-testid=rewind-button]");
    expect(rewindButton).not.toBeNull();
  });

  it("should render forward button", () => {
    const forwardButton = element.querySelector("[data-testid=forward-button]");
    expect(forwardButton).not.toBeNull();
  });
});

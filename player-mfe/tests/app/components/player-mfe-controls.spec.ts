import { beforeAll, beforeEach, describe, expect, it, vi } from "vitest";

import PlayerMFEControls from "../../../src/app/components/player-mfe-controls";
import EventBus from "../../../src/lib/event-bus/event-bus";
import { provide } from "../../../src/app/services/global-provider";

describe("PlayerMFEControls", () => {
  const socketHandlerMock = {
    playLiveStream: vi.fn(),
    pauseLiveStream: vi.fn(),
  };
  const eventBus = new EventBus();

  let element: PlayerMFEControls;

  beforeAll(() => {
    if (!customElements.get("player-mfe-controls")) {
      customElements.define("player-mfe-controls", PlayerMFEControls);
    }

    provide("eventBus", eventBus);
    provide("socketHandler", socketHandlerMock);
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

  it("should call playLiveStream() when play button is clicked", () => {
    const playButton = element.querySelector("[data-testid=play-button]")!;
    playButton.dispatchEvent(new MouseEvent("click"));

    expect(socketHandlerMock.playLiveStream).toHaveBeenCalled();
  });

  it("should call pauseLiveStream() when play button is clicked again", () => {
    const playButton = element.querySelector("[data-testid=play-button]")!;
    playButton.dispatchEvent(new MouseEvent("click"));
    playButton.dispatchEvent(new MouseEvent("click"));

    expect(socketHandlerMock.pauseLiveStream).toHaveBeenCalled();
  });

  it("should change play button icon when clicked", () => {
    const playButton = element.querySelector("[data-testid=play-button]")!;
    playButton.dispatchEvent(new MouseEvent("click"));

    expect(playButton.getAttribute("aria-label")).toBe("Pause");
  });

  it("should change play button icon when clicked again", () => {
    const playButton = element.querySelector("[data-testid=play-button]")!;
    playButton.dispatchEvent(new MouseEvent("click"));
    playButton.dispatchEvent(new MouseEvent("click"));

    expect(playButton.getAttribute("aria-label")).toBe("Play");
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

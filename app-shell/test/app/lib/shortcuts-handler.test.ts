import { afterAll, beforeAll, describe, expect, it, vi } from "vitest";

import ShortcurtsHandler from "../../../src/app/lib/shortcuts-handler";
import EventBus from "../../../src/app/lib/event-bus";

describe("ShortcurtsHandler", () => {
  let shortcutsHandler: ShortcurtsHandler;
  let eventBus: EventBus;
  let events: Record<string, (...args: any[]) => void> = {};
  let shortcuts: Record<string, Shortcut> = {
    t: { eventName: "test", data: {} },
  };

  beforeAll(() => {
    vi.stubGlobal("BroadcastChannel", vi.fn());
    eventBus = new EventBus();
    shortcutsHandler = new ShortcurtsHandler(eventBus, shortcuts);
    document.addEventListener = vi.fn().mockImplementationOnce((event, cb) => {
      events[event] = cb;
    });

    eventBus.initialize();
    shortcutsHandler.initialize();
  });

  afterAll(() => {
    vi.unstubAllGlobals();
  });

  it("should initialize the shortcuts handler", () => {
    expect(document.addEventListener).toBeCalledWith(
      "keydown",
      expect.any(Function)
    );
  });

  it("should listen to keydown events", () => {
    const spy = vi.spyOn(eventBus, "publish").mockImplementationOnce(() => {});
    events.keydown({ ctrlKey: true, key: "t", preventDefault: () => {} });

    expect(spy).toHaveBeenCalledWith("test", {});
  });

  it("should register shortcuts", () => {
    const spy = vi.spyOn(eventBus, "publish").mockImplementationOnce(() => {});
    shortcutsHandler.registerShortcut("n", "new_shortcut", {});
    events.keydown({ ctrlKey: true, key: "n", preventDefault: () => {} });

    expect(spy).toHaveBeenCalledWith("new_shortcut", {});
  });
});

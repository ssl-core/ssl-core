import { Mock, afterAll, beforeAll, describe, expect, it, vi } from "vitest";

import App from "../../src/app/app";
import Router from "../../src/app/lib/router";
import EventBus from "../../src/app/lib/event-bus";
import ShortcutsHandler from "../../src/app/lib/shortcuts-handler";
import IconsHandler from "../../src/app/lib/icons-handler";

describe("App", () => {
  let app: App;
  let routes: Route[] = [];
  let templates: Record<string, Template> = {};
  let shortcuts: Record<string, Shortcut> = {};
  let icons: Record<string, Icon> = {};
  let routerMock: Mock = vi.fn();
  let eventBusMock: Mock = vi.fn();
  let shortcutsHandlerMock: Mock = vi.fn();
  let iconsHandlerMock: Mock = vi.fn();

  beforeAll(() => {
    vi.stubGlobal("BroadcastChannel", vi.fn());
    vi.spyOn(Router.prototype, "initialize").mockImplementation(routerMock);
    vi.spyOn(EventBus.prototype, "initialize").mockImplementation(eventBusMock);
    vi.spyOn(ShortcutsHandler.prototype, "initialize").mockImplementation(
      shortcutsHandlerMock
    );
    vi.spyOn(IconsHandler.prototype, "initialize").mockImplementation(
      iconsHandlerMock
    );

    app = new App(routes, templates, shortcuts, icons);
  });

  afterAll(() => {
    vi.unstubAllGlobals();
    vi.restoreAllMocks();
  });

  it("should initialize the app", () => {
    app.initialize();

    expect(routerMock).toHaveBeenCalled();
    expect(eventBusMock).toHaveBeenCalled();
    expect(shortcutsHandlerMock).toHaveBeenCalled();
  });
});

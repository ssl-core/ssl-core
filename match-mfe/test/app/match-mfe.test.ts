import { Mock, beforeAll, describe, expect, it, vi } from "vitest";

import MatchMFE from "../../src/app/match-mfe";
import RenderEngineFactory from "../../src/app/lib/render-engine-factory";
import SocketHandler from "../../src/app/socket/socket-handler";
import BaseRenderEngine from "../../src/app/lib/base-render-engine";

describe("MatchMFE", () => {
  let engineInitializeMock: Mock = vi.fn();
  let engineTerminateMock: Mock = vi.fn();
  let socketInitializeMock: Mock = vi.fn();
  let socketTerminateMock: Mock = vi.fn();

  beforeAll(async () => {
    vi.stubGlobal(
      "Worker",
      vi.fn().mockImplementation(() => ({
        postMessage: vi.fn(),
        onmessage: vi.fn(),
        terminate: vi.fn(),
      }))
    );
    vi.spyOn(RenderEngineFactory, "createEngine").mockImplementation(
      vi.fn().mockImplementation(() => ({
        render: vi.fn(),
        initialize: engineInitializeMock,
        terminate: engineTerminateMock,
      }))
    );
    vi.spyOn(SocketHandler.prototype, "initialize").mockImplementation(
      socketInitializeMock
    );

    window.customElements.define("match-mfe", MatchMFE);
    document.body.innerHTML = "<match-mfe></match-mfe>";
  });

  it("should initialize the mfe", () => {
    expect(engineInitializeMock).toHaveBeenCalled();
    expect(socketInitializeMock).toHaveBeenCalled();
  });

  it("should terminate the mfe", () => {
    vi.spyOn(SocketHandler.prototype, "terminate").mockImplementation(
      socketTerminateMock
    );

    const mfe = document.querySelector("match-mfe") as MatchMFE;
    mfe.remove();

    expect(engineTerminateMock).toHaveBeenCalled();
    expect(socketTerminateMock).toHaveBeenCalled();
  });
});

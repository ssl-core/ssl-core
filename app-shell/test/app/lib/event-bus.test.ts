import { Mock, afterEach, beforeAll, describe, expect, it, vi } from "vitest";

import EventBus from "../../../src/app/lib/event-bus";

describe("EventBus", () => {
  let events: Record<string, (...args: any[]) => void> = {};
  let BroadcastChannelMock: Mock;
  let postMessageMock: Mock;
  let onmessageMock: Mock;
  let closeMock: Mock;
  let eventBus: EventBus;

  beforeAll(() => {
    onmessageMock = vi.fn();
    postMessageMock = vi.fn().mockImplementation((message: any) => {
      BroadcastChannelMock.mock.results[0].value.onmessage({ data: message });
    });
    closeMock = vi.fn();
    BroadcastChannelMock = vi.fn().mockImplementation(() => ({
      postMessage: postMessageMock,
      onmessage: onmessageMock,
      close: closeMock,
    }));
    vi.stubGlobal("BroadcastChannel", BroadcastChannelMock);

    window.addEventListener = vi.fn().mockImplementationOnce((event, cb) => {
      events[event] = cb;
    });

    eventBus = new EventBus();
    eventBus.initialize();
  });

  afterEach(() => {
    vi.unstubAllGlobals();
  });

  it("should initialize the event bus", () => {
    expect(BroadcastChannelMock).toHaveBeenCalledWith("ssl-event-bus");
  });

  it("should publish an event", () => {
    eventBus.publish("test", "test");

    expect(postMessageMock).toHaveBeenCalledWith({
      event: "test",
      data: "test",
    });
  });

  it("should subscribe to an event", () => {
    const testFunction = vi.fn();
    eventBus.subscribe("test", testFunction);
    eventBus.publish("test", "test");

    expect(testFunction).toHaveBeenCalledWith("test");
  });

  it("should unsubscribe from an event", () => {
    const testFunction = vi.fn();
    eventBus.subscribe("test", testFunction);
    eventBus.unsubscribe("test");
    eventBus.publish("test", "test");

    expect(testFunction).not.toHaveBeenCalled();
  });

  it("should unsubscribe from all events", () => {
    eventBus.unsubscribeAll();

    expect(closeMock).toHaveBeenCalled();
  });

  it("should close connection on page hide", () => {
    events.pagehide();

    expect(closeMock).toHaveBeenCalled();
  });
});

class ThreeProxyEventHandlers {
  private static readonly orbitKeys = [
    "ArrowDown",
    "ArrowLeft",
    "ArrowRight",
    "ArrowUp",
  ];

  public static getEventHandlers() {
    return {
      contextmenu: ThreeProxyEventHandlers.preventDefaultHandler,
      mousedown: ThreeProxyEventHandlers.mouseEventHandler,
      mousemove: ThreeProxyEventHandlers.mouseEventHandler,
      mouseup: ThreeProxyEventHandlers.mouseEventHandler,
      pointerdown: ThreeProxyEventHandlers.mouseEventHandler,
      pointermove: ThreeProxyEventHandlers.mouseEventHandler,
      pointerup: ThreeProxyEventHandlers.mouseEventHandler,
      touchstart: ThreeProxyEventHandlers.touchEventHandler,
      touchmove: ThreeProxyEventHandlers.touchEventHandler,
      touchend: ThreeProxyEventHandlers.touchEventHandler,
      wheel: ThreeProxyEventHandlers.wheelEventHandler,
      keydown: ThreeProxyEventHandlers.filteredKeydownEventHandler,
    };
  }

  private static mouseEventHandler(event: Event, send: (data: any) => void) {
    const handler = ThreeProxyEventHandlers.buildSendPropertiesHandler([
      "ctrlKey",
      "metaKey",
      "shiftKey",
      "button",
      "pointerType",
      "clientX",
      "clientY",
      "pageX",
      "pageY",
    ]);

    handler(event, send);
  }

  private static wheelEventHandler(event: Event, send: (data: any) => void) {
    event.preventDefault();
    const handler = ThreeProxyEventHandlers.buildSendPropertiesHandler([
      "deltaX",
      "deltaY",
    ]);

    handler(event, send);
  }

  private static keydownEventHandler(event: Event, send: (data: any) => void) {
    event.preventDefault();
    const handler = ThreeProxyEventHandlers.buildSendPropertiesHandler([
      "ctrlKey",
      "metaKey",
      "shiftKey",
      "keyCode",
    ]);

    handler(event, send);
  }

  private static filteredKeydownEventHandler(
    event: KeyboardEvent,
    send: (data: any) => void
  ) {
    if (ThreeProxyEventHandlers.orbitKeys.includes(event.code)) {
      ThreeProxyEventHandlers.keydownEventHandler(event, send);
    }
  }

  private static touchEventHandler(
    event: TouchEvent,
    send: (data: any) => void
  ) {
    send({
      type: event.type,
      touches: Array.from(event.touches).map((touch) => ({
        pageX: touch.pageX,
        pageY: touch.pageY,
      })),
    });
  }

  private static preventDefaultHandler(event: Event) {
    event.preventDefault();
  }

  private static copyProperties(src: any, properties: any, dst: any) {
    for (const name of properties) {
      dst[name] = src[name];
    }
  }

  private static buildSendPropertiesHandler(properties: any) {
    return (event: Event, send: (data: any) => void) => {
      const data = { type: event.type, pointerId: 0 };
      ThreeProxyEventHandlers.copyProperties(event, properties, data);
      send(data);
    };
  }
}

export default ThreeProxyEventHandlers;

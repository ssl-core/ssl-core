const mouseEventHandler = makeSendPropertiesHandler([
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

const wheelEventHandlerImpl = makeSendPropertiesHandler(["deltaX", "deltaY"]);

const keydownEventHandler = makeSendPropertiesHandler([
  "ctrlKey",
  "metaKey",
  "shiftKey",
  "keyCode",
]);

function wheelEventHandler(event: Event, sendFn: (data: any) => void) {
  event.preventDefault();
  wheelEventHandlerImpl(event, sendFn);
}

function preventDefaultHandler(event: Event) {
  event.preventDefault();
}

function copyProperties(src: any, properties: any, dst: any) {
  for (const name of properties) {
    dst[name] = src[name];
  }
}

function makeSendPropertiesHandler(properties: any) {
  return function sendProperties(event: Event, sendFn: (data: any) => void) {
    const data = { type: event.type, pointerId: 0 };
    copyProperties(event, properties, data);
    sendFn(data);
  };
}

function touchEventHandler(event: TouchEvent, sendFn: (data: any) => void) {
  const touches: any[] = [];
  const data = { type: event.type, touches };
  for (let i = 0; i < event.touches.length; ++i) {
    const touch = event.touches[i];
    touches.push({
      pageX: touch.pageX,
      pageY: touch.pageY,
    });
  }
  sendFn(data);
}

const orbitKeys: Record<string, boolean> = {
  "37": true,
  "38": true,
  "39": true,
  "40": true,
};

function filteredKeydownEventHandler(
  event: KeyboardEvent,
  sendFn: (data: any) => void
) {
  const { keyCode } = event;
  if (orbitKeys[keyCode.toString()]) {
    event.preventDefault();
    keydownEventHandler(event, sendFn);
  }
}

export const eventHandlers = {
  contextmenu: preventDefaultHandler,
  mousedown: mouseEventHandler,
  mousemove: mouseEventHandler,
  mouseup: mouseEventHandler,
  pointerdown: mouseEventHandler,
  pointermove: mouseEventHandler,
  pointerup: mouseEventHandler,
  touchstart: touchEventHandler,
  touchmove: touchEventHandler,
  touchend: touchEventHandler,
  wheel: wheelEventHandler,
  keydown: filteredKeydownEventHandler,
};

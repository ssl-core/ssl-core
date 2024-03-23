import ThreeEventTypes from "../worker/three-event-types";

class ThreeElementProxy {
  private static nextProxyId: number;
  private id: number;
  private element: HTMLElement;
  private worker: Worker;
  private eventHandlers: Record<string, Function>;

  constructor(
    element: HTMLElement,
    worker: Worker,
    eventHandlers: Record<string, Function>
  ) {
    this.id = ThreeElementProxy.getNewId();
    this.element = element;
    this.worker = worker;
    this.eventHandlers = eventHandlers;
  }

  private static getNewId() {
    if (!ThreeElementProxy.nextProxyId) {
      ThreeElementProxy.nextProxyId = 0;
    }

    return ThreeElementProxy.nextProxyId++;
  }

  public getId() {
    return this.id;
  }

  public initialize() {
    this.addProxy();
    this.addEventListeners();
  }

  private addProxy = () => {
    this.worker.postMessage({
      type: ThreeEventTypes.Proxy,
      payload: this.id,
    });
  };

  private addEventListeners = () => {
    for (const [eventName, handler] of Object.entries(this.eventHandlers)) {
      this.element.addEventListener(eventName, (event) => {
        handler(event, (data: any) => this.sendEvent(data));
      });
    }
  };

  private sendEvent = (data: any) => {
    this.worker.postMessage({
      type: ThreeEventTypes.Event,
      payload: { id: this.id, data },
    });
  };
}

export default ThreeElementProxy;

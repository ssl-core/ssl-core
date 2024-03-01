class ElementProxy {
  private static nextProxyId = 0;
  private id: number;
  private element: HTMLElement;
  private worker: Worker;
  private eventHandlers: any[];

  constructor(element: HTMLElement, worker: Worker, eventHandlers: any) {
    this.id = ElementProxy.getNewId();
    this.element = element;
    this.worker = worker;
    this.eventHandlers = eventHandlers;
  }

  private static getNewId() {
    return ElementProxy.nextProxyId++;
  }

  public initialize() {
    this.worker.postMessage({
      type: "makeProxy",
      payload: this.id,
    });

    for (const [eventName, handler] of Object.entries(this.eventHandlers)) {
      this.element.addEventListener(eventName, (event) => {
        handler(event, (data: any) => this.sendEvent(data));
      });
    }
  }

  public getId() {
    return this.id;
  }

  private sendEvent = (data: any) => {
    this.worker.postMessage({
      type: "event",
      payload: { id: this.id, data },
    });
  };
}

export default ElementProxy;

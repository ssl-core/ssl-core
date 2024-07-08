class EventBus {
  private listeners: Record<string, Function[]>;

  constructor() {
    this.listeners = {};
  }

  subscribe(event: string, callback: Function) {
    if (!this.listeners[event]) {
      this.listeners[event] = [];
    }
    this.listeners[event].push(callback);
  }

  unsubscribe(event: string, callback: Function) {
    const listeners = this.listeners[event];
    if (listeners) {
      this.listeners[event] = listeners.filter(
        (listener) => listener !== callback
      );
    }
  }

  emit(event: string, data: any) {
    const listeners = this.listeners[event];
    if (listeners) {
      listeners.forEach((callback) => {
        callback(data);
      });
    }
  }
}

export default EventBus;

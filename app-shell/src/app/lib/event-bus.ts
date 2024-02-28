class EventBus {
  private channel: BroadcastChannel;
  private subscriptions: { [key: string]: (data: any) => void } = {};

  constructor() {
    this.channel = new BroadcastChannel("ssl-event-bus");
  }

  initialize() {
    this.addMessageListener();
    this.addPageHideListener();
  }

  publish(event: string, data: any) {
    this.channel.postMessage({ event, data });
  }

  subscribe(event: string, callback: (data: any) => void) {
    this.subscriptions[event] = callback;
  }

  unsubscribe(event: string) {
    delete this.subscriptions[event];
  }

  unsubscribeAll() {
    this.channel.close();
  }

  addMessageListener() {
    this.channel.onmessage = (message) => {
      const { event, data } = message.data;
      if (this.subscriptions[event]) {
        this.subscriptions[event](data);
      }
    };
  }

  addPageHideListener() {
    window.addEventListener("pagehide", () => {
      this.unsubscribeAll();
    });
  }
}

export default EventBus;

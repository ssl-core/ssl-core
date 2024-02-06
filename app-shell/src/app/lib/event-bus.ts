class EventBus {
  private channel: BroadcastChannel;
  private subscriptions: { [key: string]: (data: any) => void } = {};

  constructor() {
    this.channel = new BroadcastChannel("ssl-event-bus");
  }

  initialize() {
    this.channel.onmessage = (message) => {
      if (this.subscriptions[message.data.event]) {
        this.subscriptions[message.data.event](message.data.data);
      }
    };
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
}

export default EventBus;

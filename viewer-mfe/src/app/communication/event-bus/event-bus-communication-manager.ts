import Channels from "../../../config/channels";
import BaseCommunicationManager from "../base-communication-manager";

class EventBusCommunicationManager extends BaseCommunicationManager {
  private eventBus: BroadcastChannel;
  private engineChannel: BroadcastChannel;

  constructor() {
    super();
    this.eventBus = new BroadcastChannel(Channels.EventBus);
    this.engineChannel = new BroadcastChannel(Channels.Engine);
  }

  public connect() {
    this.eventBus.addEventListener("message", (message) =>
      this.handleMessage(message.data as EventBusMessage)
    );
  }

  public disconnect() {
    this.eventBus.removeEventListener("message", (message) =>
      this.handleMessage(message.data as EventBusMessage)
    );
  }

  public send(message: any) {
    if (!this.eventBus) {
      throw new Error("Communication not initialized");
    }

    this.eventBus.postMessage(message);
  }

  private handleMessage(message: EventBusMessage) {
    if (message.type === "frame") {
      this.engineChannel.postMessage(message.payload);
    }
  }
}

export default EventBusCommunicationManager;

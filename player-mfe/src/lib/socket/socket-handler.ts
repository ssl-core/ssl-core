import SocketWorker from "../../workers/socket-worker?worker";
import EventBus from "../event-bus/event-bus";

class SocketHandler {
  private worker: Worker;
  private address: string;
  private eventBus: EventBus;

  constructor(address: string, eventBus: EventBus) {
    this.worker = new SocketWorker();
    this.address = address;
    this.eventBus = eventBus;
  }

  public initialize() {
    this.worker.postMessage({
      type: "connect",
      payload: { address: this.address },
    });

    this.handleWorkerMessages();
  }

  public terminate() {
    this.worker.terminate();
  }

  private handleWorkerMessages() {
    this.worker.onmessage = (event) => {
      this.eventBus.emit("frame", event.data);
    };
  }
}

export default SocketHandler;

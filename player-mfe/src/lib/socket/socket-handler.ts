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

  public connect() {
    this.send("connect", { address: this.address });
    this.handleWorkerMessages();
  }

  public disconnect() {
    this.worker.terminate();
  }

  public playLiveStream() {
    this.send("play");
  }

  public pauseLiveStream() {
    this.send("pause");
  }

  private send(type: string, payload: any = {}) {
    this.worker.postMessage({ type, payload });
  }

  private handleWorkerMessages() {
    this.worker.onmessage = (event) => {
      this.eventBus.emit("frame", event.data);
    };
  }
}

export default SocketHandler;

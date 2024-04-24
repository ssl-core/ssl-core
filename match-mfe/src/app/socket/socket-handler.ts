import SocketWorker from "./worker/socket-worker?worker";
import SocketEventTypes from "./worker/socket-event-types";

class SocketHandler {
  private worker: Worker;
  private address: string;

  constructor(address: string) {
    this.worker = new SocketWorker();
    this.address = address;
  }

  public initialize() {
    this.worker.postMessage({
      type: SocketEventTypes.Connect,
      payload: { address: this.address },
    });
  }

  public terminate() {
    this.worker.terminate();
  }
}

export default SocketHandler;

import CommunicationWorker from "./worker/communication-worker?worker&inline";
import CommunicationEventTypes from "./worker/communication-event-types";

class CommunicationHandler {
  private worker: Worker;
  private address: string;

  constructor(address: string) {
    this.worker = new CommunicationWorker();
    this.address = address;
  }

  public initialize() {
    this.worker.postMessage({
      type: CommunicationEventTypes.Connect,
      payload: { address: this.address },
    });
  }

  public terminate() {
    this.worker.terminate();
  }
}

export default CommunicationHandler;

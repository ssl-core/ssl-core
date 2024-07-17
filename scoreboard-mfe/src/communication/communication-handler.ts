import CommunicationWorker from "../workers/communication-worker?worker&inline";

class CommunicationHandler {
  private worker: Worker | null;

  constructor() {
    this.worker = null;
  }

  public initialize() {
    this.worker = new CommunicationWorker();
  }

  public terminate() {
    this.worker?.terminate();
  }
}

export default CommunicationHandler;

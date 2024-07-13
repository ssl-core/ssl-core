import SocketWorker from "../../workers/socket-worker?worker&inline";

class SocketHandler {
  private worker: Worker;
  private address: string;
  private listeners: Record<string, Function[]>;

  constructor(address: string) {
    this.worker = new SocketWorker();
    this.address = address;
    this.listeners = {};
  }

  public connect() {
    this.send("connect", { address: this.address });
    this.handleWorkerMessages();
  }

  public disconnect() {
    this.send("disconnect");
    this.worker.terminate();
  }

  public playLiveStream() {
    this.send("play");
  }

  public pauseLiveStream() {
    this.send("pause");
  }

  public fetchReplay(timestamp: number) {
    this.send("fetch", { timestamp });
  }

  public addEventListener(event: string, callback: Function) {
    if (!this.listeners[event]) {
      this.listeners[event] = [];
    }

    this.listeners[event].push(callback);
  }

  public removeEventListener(event: string, callback: Function) {
    if (!this.listeners[event]) {
      return;
    }

    this.listeners[event] = this.listeners[event].filter(
      (listener) => listener !== callback
    );
  }

  private send(type: string, payload: any = {}) {
    this.worker.postMessage({ type, payload });
  }

  private handleWorkerMessages() {
    this.worker.onmessage = (event) => {
      const { type, payload } = event.data;
      this.dispatchEvent(type, payload);
    };
  }

  private dispatchEvent(event: string, payload: any) {
    if (!this.listeners[event]) {
      return;
    }

    this.listeners[event].forEach((listener) => {
      listener(payload);
    });
  }
}

export default SocketHandler;

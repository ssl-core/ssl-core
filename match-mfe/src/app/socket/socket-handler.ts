import BaseRenderEngine from "../lib/base-render-engine";
import SocketWorker from "./worker/socket-worker?worker";

class SocketHandler {
  private worker: Worker;
  private renderEngine: BaseRenderEngine;

  constructor(renderEngine: BaseRenderEngine) {
    this.worker = new SocketWorker();
    this.renderEngine = renderEngine;
  }

  public initialize() {
    this.addMessageListener();
  }

  public terminate() {
    this.worker.terminate();
  }

  private addMessageListener() {
    this.worker.addEventListener("message", (event) => {
      this.renderEngine.render(event.data);
    });
  }
}

export default SocketHandler;

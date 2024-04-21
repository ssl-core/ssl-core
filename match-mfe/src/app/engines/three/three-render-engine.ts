import BaseRenderEngine from "../base-render-engine";
import ThreeWorker from "./worker/three-worker?worker";
import ThreeElementProxy from "./proxy/three-element-proxy";
import ThreeProxyEventHandlers from "./proxy/three-proxy-event-handlers";
import ThreeEventTypes from "./worker/three-event-types";

class ThreeRenderEngine extends BaseRenderEngine {
  private parentElement: HTMLElement;
  private canvas: HTMLCanvasElement;
  private worker: Worker;
  private observer: ResizeObserver | null;

  constructor(root: ShadowRoot) {
    super(root);

    this.parentElement = root.host.parentElement!;
    this.canvas = document.createElement("canvas");
    this.worker = new ThreeWorker();
    this.observer = null;
  }

  public initialize() {
    this.appendCanvas();
    this.initializeWorker();
    this.addResizeObserver();
    this.proxyWorkerMessages();
  }

  public terminate() {
    this.worker.terminate();
    this.observer?.disconnect();
  }

  public render(frame: Frame) {
    this.worker.postMessage({ type: ThreeEventTypes.Frame, payload: frame });
  }

  private appendCanvas() {
    this.canvas.width = this.parentElement.clientWidth;
    this.canvas.height = this.parentElement.clientHeight;
    this.root.appendChild(this.canvas);
  }

  private initializeWorker() {
    const offscreen = this.canvas.transferControlToOffscreen();
    const eventHandlers = ThreeProxyEventHandlers.getEventHandlers();
    const proxy = new ThreeElementProxy(
      this.canvas,
      this.worker,
      eventHandlers
    );
    proxy.initialize();

    this.worker.postMessage(
      {
        type: ThreeEventTypes.Initialize,
        payload: { canvas: offscreen, proxyId: proxy.getId() },
      },
      [offscreen]
    );
  }

  private addResizeObserver() {
    const observer = new ResizeObserver((entries) => {
      const { width, height, top, left } = entries[0].contentRect;
      this.worker.postMessage({
        type: ThreeEventTypes.Resize,
        payload: { width, height, top, left },
      });
    });

    observer.observe(this.parentElement);
  }

  private proxyWorkerMessages() {
    let lastTimestamp = 0;
    this.worker.onmessage = (message) => {
      if (lastTimestamp === 0) {
        lastTimestamp = message.data;
        return;
      }

      const event = new CustomEvent("tester", {
        detail: {
          duration: message.data - lastTimestamp,
          startTime: lastTimestamp,
          endTime: message.data,
        },
      });

      window.document.body.dispatchEvent(event);
      lastTimestamp = message.data;
    };
  }
}

export default ThreeRenderEngine;

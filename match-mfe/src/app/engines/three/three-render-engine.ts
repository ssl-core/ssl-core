import BaseRenderEngine from "../../lib/base-render-engine";
import ThreeWorker from "./worker/three-worker?worker";
import ElementProxy from "./proxy/three-element-proxy";
import { eventHandlers } from "./proxy/three-proxy-event-handlers";

class ThreeRenderEngine extends BaseRenderEngine {
  private parentElement: HTMLElement;
  private canvas: HTMLCanvasElement;
  private worker: Worker;
  private observer?: ResizeObserver;

  constructor(root: ShadowRoot) {
    super(root);

    this.parentElement = root.host.parentElement!;
    this.canvas = document.createElement("canvas");
    this.worker = new ThreeWorker();
  }

  public initialize() {
    this.appendCanvas();
    this.initializeWorker();
    this.addResizeObserver();
  }

  public terminate() {
    this.worker.terminate();
    this.observer?.disconnect();
  }

  public render(match: Match) {
    this.worker.postMessage({ type: "frame", payload: match });
  }

  private appendCanvas() {
    this.canvas.width = this.parentElement.clientWidth;
    this.canvas.height = this.parentElement.clientHeight;
    this.root.appendChild(this.canvas);
  }

  private initializeWorker() {
    const offscreen = this.canvas.transferControlToOffscreen();
    const proxy = new ElementProxy(this.canvas, this.worker, eventHandlers);
    proxy.initialize();

    this.worker.postMessage(
      {
        type: "initialize",
        payload: { canvas: offscreen, canvasId: proxy.getId() },
      },
      [offscreen]
    );
  }

  private addResizeObserver() {
    const observer = new ResizeObserver((entries) => {
      const { width, height, top, left } = entries[0].contentRect;
      this.worker.postMessage({
        type: "resize",
        payload: { width, height, top, left },
      });
    });

    observer.observe(this.parentElement);
  }
}

export default ThreeRenderEngine;

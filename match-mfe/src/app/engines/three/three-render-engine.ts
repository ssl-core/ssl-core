import BaseRenderEngine from "../../lib/base-render-engine";
import ThreeWorker from "./worker/three-worker?worker";

class ThreeRenderEngine extends BaseRenderEngine {
  private canvas: HTMLCanvasElement;
  private worker: Worker;

  constructor(root: ShadowRoot) {
    super(root);

    this.canvas = document.createElement("canvas");
    this.worker = new ThreeWorker();
  }

  public initialize(): void {
    this.canvas.width = window.innerWidth;
    this.canvas.height = window.innerHeight;
    this.root.appendChild(this.canvas);

    const offscreen = this.canvas.transferControlToOffscreen();
    this.worker.postMessage({ type: "initialize", payload: offscreen }, [
      offscreen,
    ]);

    this.addResizeListener();
  }

  public render(match: Match): void {
    this.worker.postMessage({ type: "frame", payload: match });
  }

  private addResizeListener() {
    window.addEventListener("resize", () => {
      this.worker.postMessage({
        type: "resize",
        payload: {
          width: window.innerWidth,
          height: window.innerHeight,
        },
      });
    });
  }
}

export default ThreeRenderEngine;

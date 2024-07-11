import BaseRenderEngine from "../base-render-engine";
import ThreeWorker from "./worker/three-worker?worker&inline";
import ThreeElementProxy from "./proxy/three-element-proxy";
import ThreeProxyEventHandlers from "./proxy/three-proxy-event-handlers";
import ThreeEventTypes from "./worker/three-event-types";
import OrientationGizmo from "./lib/orientation-gizmo";
import Stats from "./lib/stats";

class ThreeRenderEngine extends BaseRenderEngine {
  private gui: HTMLElement;
  private canvas: HTMLCanvasElement;
  private worker: Worker;
  private observer: ResizeObserver | null;
  private orientationGizmo: OrientationGizmo;
  private stats: Stats;

  constructor(root: ShadowRoot) {
    super(root);

    this.gui = document.createElement("div");
    this.canvas = document.createElement("canvas");
    this.worker = new ThreeWorker();
    this.observer = null;
    this.orientationGizmo = new OrientationGizmo(this.gui);
    this.stats = new Stats(this.gui);
  }

  public initialize() {
    this.appendCanvas();
    this.appendGUI();
    this.initializeWorker();
    this.initializeOrientationGizmo();
    this.initializeStats();
    this.enablePointerEvents();
    this.addResizeObserver();
    this.listenSyncMessages();
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

  private appendGUI() {
    this.gui.style.position = "absolute";
    this.gui.style.top = "0";
    this.gui.style.left = "0";
    this.gui.style.width = "100%";
    this.gui.style.height = "100%";
    this.gui.style.zIndex = "100";
    this.gui.style.pointerEvents = "none";
    this.root.appendChild(this.gui);
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

  private initializeOrientationGizmo() {
    this.orientationGizmo.initialize();
    this.orientationGizmo.setOnAxisSelected((axis) => {
      this.worker.postMessage({
        type: ThreeEventTypes.AxisSelected,
        payload: axis,
      });
    });
  }

  private initializeStats() {
    this.stats.initialize();
  }

  private addResizeObserver() {
    const observer = new ResizeObserver((entries) => {
      this.worker.postMessage({
        type: ThreeEventTypes.Resize,
        payload: entries[0].target.getBoundingClientRect(),
      });
    });

    observer.observe(this.parentElement);
  }

  private listenSyncMessages() {
    this.worker.addEventListener(
      "message",
      (event: MessageEvent<ThreeSyncMessage>) => {
        this.orientationGizmo.update(event.data.camera);
        this.stats.update();
      }
    );
  }

  private enablePointerEvents() {
    for (const child of this.gui.children) {
      if (child instanceof HTMLElement) {
        child.style.pointerEvents = "auto";
      }
    }
  }
}

export default ThreeRenderEngine;

import ThreeProxyManager from "./three-proxy-manager";
import ThreeSceneManager from "./three-scene-manager";
import ElementProxyReceiver from "./three-element-proxy-receiver";

class ThreeEventHandler {
  private sceneManager: ThreeSceneManager;
  private proxyManager: ThreeProxyManager;

  constructor(
    sceneManager: ThreeSceneManager,
    proxyManager: ThreeProxyManager
  ) {
    this.sceneManager = sceneManager;
    this.proxyManager = proxyManager;
  }

  public handleEvent(type: string, payload: any) {
    switch (type) {
      case "initialize":
        const proxy = this.proxyManager.getProxy(payload.canvasId);
        this.sceneManager.initialize(
          payload.canvas as OffscreenCanvas,
          proxy as ElementProxyReceiver
        );
        break;
      case "resize":
        this.sceneManager.resize(payload.width, payload.height);
        break;
      case "frame":
        this.sceneManager.renderFrame(payload as Match);
        break;
      case "makeProxy":
        this.proxyManager.makeProxy(payload as number);
        break;
      case "event":
        this.proxyManager.handleEvent(payload as { id: number; data: any });
        break;
      default:
        throw new Error(`Unknown event type: ${type}`);
    }
  }
}

export default ThreeEventHandler;

import ThreeProxyManager from "../proxy/three-proxy-manager";
import ThreeSceneManager from "../scene/three-scene-manager";

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
        const { canvas, proxyId } = payload;
        const proxy = this.proxyManager.getProxy(proxyId);

        this.sceneManager.initialize(canvas, proxy);
        break;
      case "resize":
        this.sceneManager.resize(payload.width, payload.height);
        break;
      case "frame":
        this.sceneManager.render(payload as Frame);
        break;
      case "proxy":
        this.proxyManager.addProxy(payload as number);
        break;
      case "event":
        this.proxyManager.handleEvent(payload as ProxyEvent);
        break;
      default:
        throw new Error(`Unknown event type: ${type}`);
    }
  }
}

export default ThreeEventHandler;

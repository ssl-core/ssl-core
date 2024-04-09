import ThreeProxyManager from "../proxy/three-proxy-manager";
import ThreeSceneManager from "../scene/three-scene-manager";
import ThreeEventTypes from "./three-event-types";

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
      case ThreeEventTypes.Initialize:
        const { canvas, proxyId } = payload;
        const proxy = this.proxyManager.getProxy(proxyId);

        this.sceneManager.initialize(canvas, proxy);
        break;
      case ThreeEventTypes.Resize:
        this.sceneManager.resize(payload.width, payload.height);
        break;
      case ThreeEventTypes.Frame:
        self.requestAnimationFrame(() => {
          this.sceneManager.render(payload as Frame);
        });
        break;
      case ThreeEventTypes.Proxy:
        this.proxyManager.addProxy(payload as number);
        break;
      case ThreeEventTypes.Event:
        this.proxyManager.handleEvent(payload as ProxyEvent);
        break;
      default:
        throw new Error(`Unknown event type: ${type}`);
    }
  }
}

export default ThreeEventHandler;

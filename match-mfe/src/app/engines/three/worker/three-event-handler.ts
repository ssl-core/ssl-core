import ThreeSceneManager from "./three-scene-manager";

class ThreeEventHandler {
  private sceneManager: ThreeSceneManager;

  constructor(sceneManager: ThreeSceneManager) {
    this.sceneManager = sceneManager;
  }

  public handleEvent(type: string, payload: any) {
    switch (type) {
      case "initialize":
        this.sceneManager.initialize(payload as OffscreenCanvas);
        break;
      case "resize":
        this.sceneManager.resize(payload.width, payload.height);
        break;
      case "frame":
        this.sceneManager.renderFrame(payload as Match);
        break;
      default:
        throw new Error(`Unknown event type: ${type}`);
    }
  }
}

export default ThreeEventHandler;

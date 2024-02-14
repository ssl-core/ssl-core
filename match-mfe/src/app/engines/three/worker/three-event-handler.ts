import ThreeRenderer from "./three-renderer";

class ThreeEventHandler {
  private renderer: ThreeRenderer;

  constructor(renderer: ThreeRenderer) {
    this.renderer = renderer;
  }

  public handleEvent(type: string, payload: any) {
    switch (type) {
      case "initialize":
        this.renderer.initialize(payload as OffscreenCanvas);
        break;
      case "resize":
        this.renderer.resize(payload.width, payload.height);
        break;
      case "frame":
        this.renderer.renderFrame(payload as Match);
        break;
      default:
        throw new Error(`Unknown event type: ${type}`);
    }
  }
}

export default ThreeEventHandler;

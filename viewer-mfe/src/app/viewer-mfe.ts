import BaseRenderEngine from "./engines/base-render-engine";
import RenderEngineFactory from "./engines/render-engine-factory";
import SocketHandler from "./communication/communication-handler";
import environment from "../config/environment";

class ViewerMFE extends HTMLElement {
  private root: ShadowRoot;
  private renderEngine: BaseRenderEngine;
  private socketHandler: SocketHandler;

  constructor() {
    super();
    this.root = this.attachShadow({ mode: "open" });
    this.renderEngine = RenderEngineFactory.createEngine(
      environment.engine,
      this.root
    );
    this.socketHandler = new SocketHandler(environment.socketUrl);
  }

  public connectedCallback() {
    this.renderEngine.initialize();
    this.socketHandler.initialize();
  }

  public disconnectedCallback() {
    this.socketHandler.terminate();
    this.renderEngine.terminate();
  }
}

export default ViewerMFE;

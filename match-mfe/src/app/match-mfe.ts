import BaseRenderEngine from "./lib/base-render-engine";
import RenderEngineFactory from "./lib/render-engine-factory";
import SocketHandler from "./socket/socket-handler";
import environment from "../config/environment";

class MatchMFE extends HTMLElement {
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
    this.socketHandler = new SocketHandler(this.renderEngine);
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

export default MatchMFE;

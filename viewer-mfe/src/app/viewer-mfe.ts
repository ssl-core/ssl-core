import BaseRenderEngine from "./engines/base-render-engine";
import RenderEngineFactory from "./engines/render-engine-factory";
import CommunicationHandler from "./communication/communication-handler";
import environment from "../config/environment";

class ViewerMFE extends HTMLElement {
  private root: ShadowRoot;
  private renderEngine: BaseRenderEngine;
  private communicationHandler: CommunicationHandler;

  constructor() {
    super();
    this.root = this.attachShadow({ mode: "open" });
    this.renderEngine = RenderEngineFactory.createEngine(
      environment.engine,
      this.root
    );
    this.communicationHandler = new CommunicationHandler(environment.socketUrl);
  }

  public connectedCallback() {
    this.renderEngine.initialize();
    this.communicationHandler.initialize();
  }

  public disconnectedCallback() {
    this.communicationHandler.terminate();
    this.renderEngine.terminate();
  }
}

export default ViewerMFE;

import BaseRenderEngine from "./lib/base-render-engine";
import RenderEngineFactory from "./lib/render-engine-factory";
import environment from "../config/environment";

class MatchMFE extends HTMLElement {
  private root: ShadowRoot;
  private renderer: BaseRenderEngine;

  constructor() {
    super();
    this.root = this.attachShadow({ mode: "open" });
    this.renderer = RenderEngineFactory.createEngine(
      environment.engine,
      this.root
    );
  }

  public connectedCallback() {
    this.initialize();
  }

  private initialize() {
    this.renderer.initialize();

    window.setInterval(() => {
      this.renderer.render({ id: "1" });
    }, 10);
  }
}

export default MatchMFE;

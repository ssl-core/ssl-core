import config from "../config/config";
import BaseRenderEngine from "./lib/base-render-engine";
import RenderEngineFactory from "./lib/render-engine-factory";

class MatchMFE extends HTMLElement {
  private root: ShadowRoot;
  private renderer: BaseRenderEngine;

  constructor() {
    super();
    this.root = this.attachShadow({ mode: "open" });
    this.renderer = RenderEngineFactory.createEngine(config.engine, this.root);
  }

  public connectedCallback() {
    this.initialize();
  }

  public initialize() {
    this.renderer.initialize();
  }
}

export default MatchMFE;

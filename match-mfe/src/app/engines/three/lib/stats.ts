import StatsJS from "three/examples/jsm/libs/stats.module.js";

class Stats {
  private statsjs = new StatsJS();

  constructor(parent: ShadowRoot | HTMLElement) {
    this.statsjs.dom.style.position = "absolute";
    this.statsjs.dom.style.top = "0px";
    parent.appendChild(this.statsjs.dom);
  }

  public initialize() {
    this.statsjs.showPanel(0);
  }

  public update() {
    this.statsjs.update();
  }
}

export default Stats;

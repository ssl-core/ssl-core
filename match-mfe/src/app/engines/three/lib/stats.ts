import StatsJS from "three/addons/libs/stats.module.js";

class Stats {
  private statsjs: StatsJS;

  constructor(parent: ShadowRoot | HTMLElement) {
    this.statsjs = new StatsJS();
    this.statsjs.dom.style.position = "absolute";
    this.statsjs.dom.style.top = "0.5rem";
    this.statsjs.dom.style.left = "0.5rem";
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

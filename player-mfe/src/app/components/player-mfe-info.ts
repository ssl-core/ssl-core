import { html } from "../../utils/literals";
import { liveIcon } from "../../utils/icons";

class PlayerMFEInfo extends HTMLElement {
  constructor() {
    super();
  }

  public connectedCallback() {
    this.render();
  }

  public disconnectedCallback() {}

  public render() {
    this.innerHTML = html`
      <div class="info">
        <div class="info__type">${liveIcon}</div>
        <div class="info__container">
          <span class="info__container__title">Live</span>
          <span class="info__container__subtitle">
            RobôCIn x Tigers Mannheim
          </span>
        </div>
      </div>
    `;
  }
}

customElements.define("player-mfe-info", PlayerMFEInfo);

export default PlayerMFEInfo;

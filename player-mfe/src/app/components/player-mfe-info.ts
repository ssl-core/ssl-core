import { html } from "../../utils/literals";

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
        <button class="info__live-button" aria-label="Toggle live">
          <div class="info__live-button__circle"></div>
          <span class="info__live-button__text">Live</span>
        </button>
      </div>
    `;
  }
}

customElements.define("player-mfe-info", PlayerMFEInfo);

export default PlayerMFEInfo;

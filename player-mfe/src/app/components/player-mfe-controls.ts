import { html } from "../../utils/literals";
import { forwardIcon, playIcon, rewindIcon } from "../../utils/icons";

class PlayerMFEControls extends HTMLElement {
  constructor() {
    super();
  }

  public connectedCallback() {
    this.render();
  }

  public disconnectedCallback() {}

  public render() {
    this.innerHTML = html`
      <div class="controls">
        <button class="controls__button">${rewindIcon}</button>
        <button class="controls__play">${playIcon}</button>
        <button class="controls__button">${forwardIcon}</button>
      </div>
    `;
  }
}

customElements.define("player-mfe-controls", PlayerMFEControls);

export default PlayerMFEControls;

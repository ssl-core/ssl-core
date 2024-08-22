import { html } from "../../utils/literals";
import { maximizeIcon, settingsIcon, uploadIcon } from "../../utils/icons";

class PlayerMFEActions extends HTMLElement {
  constructor() {
    super();
  }

  public connectedCallback() {
    this.render();
    this.querySelectorAll(".actions__button").forEach((element) =>
      element.addEventListener("click", () => {
        alert("Not implemented yet");
      })
    );
  }

  public disconnectedCallback() {
    this.querySelectorAll(".actions__button").forEach((element) =>
      element.removeEventListener("click", () => {
        alert("Not implemented yet");
      })
    );
  }

  public render() {
    this.innerHTML = html`
      <div class="actions">
        <button class="actions__button">${uploadIcon}</button>
        <button class="actions__button">${settingsIcon}</button>
        <button class="actions__button">${maximizeIcon}</button>
      </div>
    `;
  }
}

customElements.define("player-mfe-actions", PlayerMFEActions);

export default PlayerMFEActions;

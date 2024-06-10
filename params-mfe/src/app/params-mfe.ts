import { html } from "../utils/literals";

import style from "./styles/main.css?inline";
import closeIcon from "./assets/close.svg";

class ParamsMFE extends HTMLElement {
  private root: ShadowRoot;
  private params: { [key: string]: unknown };
  private dialog: HTMLDialogElement | null;

  constructor() {
    super();
    this.root = this.attachShadow({ mode: "open" });
    this.params = {
      ip: null,
      port: null,
    };
    this.dialog = null;

    this.render();
  }

  public connectedCallback() {
    this.dialog = this.root.querySelector("dialog") as HTMLDialogElement;
    this.addEventListeners();
    this.showModal();
  }

  public disconnectedCallback() {}

  public render() {
    this.root.innerHTML = html`
      <dialog class="dialog">
        <h2 class="dialog__title">Parameters</h2>
        <div class="dialog__subtitle">Fill with the connection info</div>
        <button class="dialog__close">${closeIcon}</button>
        <div class="dialog__field">
          <label for="ip" class="dialog__field__label">IP</label>
          <input
            id="ip"
            type="text"
            placeholder="Enter the IP address"
            class="dialog__field__input"
          />
        </div>
        <div class="dialog__field">
          <label for="port" class="dialog__field__label">Port</label>
          <input
            id="port"
            type="text"
            placeholder="Enter the port"
            class="dialog__field__input"
          />
        </div>
        <button class="dialog__button">Save</button>
      </dialog>
      <style>
        ${style}
      </style>
    `;
  }

  public showModal() {
    this.dialog?.showModal();
  }

  public addEventListeners() {
    this.dialog?.querySelector("button")?.addEventListener("click", () => {
      this.dialog?.close();
    });
  }
}

export default ParamsMFE;

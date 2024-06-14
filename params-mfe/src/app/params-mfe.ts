import { html } from "../utils/literals";

import style from "./styles/main.css?inline";
import closeIcon from "./assets/close.svg";
import { params } from "../config/params";
import { env } from "../config/env";

class ParamsMFE extends HTMLElement {
  private root: ShadowRoot;
  private params: { [key: string]: unknown };
  private dialog: HTMLDialogElement | null;

  constructor() {
    super();
    this.root = this.attachShadow({ mode: "open" });
    this.params = Object.fromEntries(params.map((param) => [param.id, null]));
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
        ${params
          .map(
            (param) => html`
              <div class="dialog__field">
                <label for="${param.id}" class="dialog__field__label">
                  ${param.label}
                </label>
                <input
                  id="${param.id}"
                  type="text"
                  placeholder="${param.placeholder}"
                  class="dialog__field__input"
                />
              </div>
            `
          )
          .join("")}
        <button class="dialog__button">Connect</button>
      </dialog>
      <style>
        ${style}
      </style>
    `;
  }

  private showModal() {
    this.dialog?.showModal();
  }

  private addEventListeners() {
    for (const param of Object.keys(this.params)) {
      this.shadowRoot
        ?.querySelector<HTMLInputElement>(`#${param}`)
        ?.addEventListener("change", (event) => {
          const target = event.target as HTMLInputElement;
          this.params[param] = target.value;
        });
    }

    this.shadowRoot
      ?.querySelector(".dialog__close")
      ?.addEventListener("click", async () => {
        this.dialog?.close();
      });

    this.shadowRoot
      ?.querySelector(".dialog__button")
      ?.addEventListener("click", async () => {
        await this.sendParams();
        this.dialog?.close();
      });
  }

  private async sendParams() {
    await fetch(`${env.baseUrl}/params`, {
      method: "POST",
      body: JSON.stringify(this.params),
      headers: {
        "Content-Type": "application/json",
      },
    });
  }
}

export default ParamsMFE;

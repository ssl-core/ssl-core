import React from "react";
import ReactDOM from "react-dom";

import App from "./App";

class DrawMFE extends HTMLElement {
  constructor() {
    super();
    this._shadowRoot = this.attachShadow({ "mode": "open" });
  }

  connectedCallback() {
    ReactDOM.render(<App />, this._shadowRoot);
  }
}

const tagName = "draw-mfe";

if (!window.customElements.get(tagName)) {
  window.customElements.define(tagName, DrawMFE);
}

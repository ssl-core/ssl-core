// import r2wc from "@r2wc/react-to-web-component"

// const Parameters = r2wc(App)

// customElements.define("parameters-mfe", Parameters)


import ReactDOM from "react-dom/client";
import App from './App'

class Parameters extends HTMLElement {
  mountPoint!: HTMLSpanElement;
  name!: string;

  connectedCallback() {
    const mountPoint = document.createElement("div");
    this.attachShadow({ mode: "open" }).appendChild(mountPoint);

    ReactDOM.createRoot(mountPoint).render(<App />);
  }
}
export default Parameters;

customElements.define("parameters-mfe", Parameters);

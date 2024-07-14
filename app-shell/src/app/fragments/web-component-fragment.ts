import BaseFragment from "./base-fragment";

class WebComponentFragment extends BaseFragment {
  private element: HTMLElement | null;
  private script: HTMLScriptElement | null;

  constructor(
    url: string,
    container: HTMLElement,
    metadata: Record<string, any>
  ) {
    super(url, container, metadata);
    this.element = null;
    this.script = null;
  }

  render() {
    this.element = document.createElement(this.metadata.tag);
    this.container.appendChild(this.element!);

    this.script = document.createElement("script");
    this.script.type = "module";
    this.script.crossOrigin = "";
    this.script.src = this.url;

    document.body.appendChild(this.script);
  }

  clear() {
    this.element?.remove();
    this.script?.remove();
  }
}

export default WebComponentFragment;

import BaseFragment from "./base-fragment";

class IFrameFragment extends BaseFragment {
  private iframe: HTMLIFrameElement | null;

  constructor(
    url: string,
    container: HTMLElement,
    metadata: Record<string, any>
  ) {
    super(url, container, metadata);
    this.iframe = null;
  }

  render() {
    this.iframe = document.createElement("iframe");
    this.iframe.src = this.url;
    this.container.appendChild(this.iframe);
  }

  clear() {
    this.iframe?.remove();
  }
}

export default IFrameFragment;

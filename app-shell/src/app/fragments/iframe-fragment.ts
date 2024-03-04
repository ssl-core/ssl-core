import BaseFragment from "./base-fragment";

class IFrameFragment extends BaseFragment {
  render(): void {
    const iframe = document.createElement("iframe");
    iframe.src = this.url;
    this.container.appendChild(iframe);
  }
}

export default IFrameFragment;

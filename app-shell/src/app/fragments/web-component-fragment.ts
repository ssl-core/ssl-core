import BaseFragment from "./base-fragment";

class WebComponentFragment extends BaseFragment {
  render(): void {
    const element = document.createElement(this.metadata.tag);
    this.container.appendChild(element);

    const script = document.createElement("script");
    script.type = "module";
    script.crossOrigin = "";
    script.src = this.url;

    document.body.appendChild(script);
  }
}

export default WebComponentFragment;

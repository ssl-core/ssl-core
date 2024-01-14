import BaseFragment from "./base_fragment";

class WebComponentFragment extends BaseFragment {
  render(): void {
    const script = document.createElement("script");
    script.type = "module";
    script.src = this.url;
    script.onload = () => {
      const element = document.createElement(this.metadata.tag);
      this.container.appendChild(element);
    };

    document.body.appendChild(script);
  }
}

export default WebComponentFragment;

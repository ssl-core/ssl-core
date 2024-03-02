import { EventDispatcher } from "three";

class ElementProxyReceiver extends EventDispatcher {
  private width: number;
  private height: number;
  private left: number;
  private top: number;
  public style: any;
  public ownerDocument: any;

  constructor() {
    super();

    this.width = 0;
    this.height = 0;
    this.left = 0;
    this.top = 0;
    this.style = {};
    this.ownerDocument = this;
  }

  get clientWidth() {
    return this.width;
  }

  get clientHeight() {
    return this.height;
  }

  public handleEvent(data: any) {
    data.preventDefault = () => {};
    data.stopPropagation = () => {};

    // @ts-ignore
    this.dispatchEvent<any>(data);
  }

  public setSize(data: any) {
    this.left = data.left;
    this.top = data.top;
    this.width = data.width;
    this.height = data.height;
  }

  public getRootNode() {
    return this;
  }

  public setPointerCapture() {}

  public releasePointerCapture() {}

  public getBoundingClientRect() {
    return {
      left: this.left,
      top: this.top,
      width: this.width,
      height: this.height,
      right: this.left + this.width,
      bottom: this.top + this.height,
      x: this.left,
      y: this.top,
      toJSON: () => {},
    };
  }

  public focus() {}
}

export default ElementProxyReceiver;

import ElementProxyReceiver from "./three-element-proxy-receiver";

class ThreeProxyManager {
  private targets: Record<number, ElementProxyReceiver>;

  constructor() {
    this.targets = {};
    this.handleEvent = this.handleEvent.bind(this);
  }

  public makeProxy(id: number) {
    const proxy = new ElementProxyReceiver();
    this.targets[id] = proxy;
  }

  public getProxy(id: number) {
    return this.targets[id];
  }

  public handleEvent(data: { id: number; data: any }) {
    this.targets[data.id].handleEvent(data.data);
  }
}

export default ThreeProxyManager;

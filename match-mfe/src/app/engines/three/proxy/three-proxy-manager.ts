import ThreeElementProxyReceiver from "./three-element-proxy-receiver";

class ThreeProxyManager {
  private targets: Record<number, ThreeElementProxyReceiver>;

  constructor() {
    this.targets = {};
  }

  public addProxy(id: number) {
    const proxy = new ThreeElementProxyReceiver();
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

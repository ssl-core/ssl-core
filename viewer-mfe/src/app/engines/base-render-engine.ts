abstract class BaseRenderEngine {
  protected root: ShadowRoot;
  protected parentElement: HTMLElement;

  constructor(root: ShadowRoot) {
    this.root = root;
    this.parentElement = root.host.parentElement!;
  }

  public abstract initialize(): void;
  public abstract terminate(): void;
  public abstract render(frame: Frame): void;
}

export default BaseRenderEngine;

abstract class BaseRenderEngine {
  protected root: ShadowRoot;

  constructor(root: ShadowRoot) {
    this.root = root;
  }

  public abstract initialize(): void;
  public abstract terminate(): void;
  public abstract render(frame: Frame): void;
}

export default BaseRenderEngine;

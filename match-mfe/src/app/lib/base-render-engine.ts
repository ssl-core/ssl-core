abstract class BaseRenderEngine {
  protected root: ShadowRoot;

  constructor(root: ShadowRoot) {
    this.root = root;
  }

  public abstract initialize(): void;
  public abstract render(match: Match): void;
}

export default BaseRenderEngine;

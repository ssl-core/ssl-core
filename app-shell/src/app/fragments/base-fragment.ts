abstract class BaseFragment {
  protected url: string;
  protected container: HTMLElement;
  protected metadata: Record<string, any>;

  constructor(
    url: string,
    container: HTMLElement,
    metadata: Record<string, any>
  ) {
    this.url = url;
    this.container = container;
    this.metadata = metadata;
  }

  abstract render(): void;
  abstract clear(): void;
}

export default BaseFragment;

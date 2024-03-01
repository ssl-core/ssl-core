import { Group, Mesh } from "three";

abstract class ThreeBaseObject extends Group {
  protected metadata: Record<string, any>;

  constructor(metadata?: Record<string, any>) {
    super();

    this.metadata = metadata || {};
    this.buildMeshes().forEach((mesh) => this.add(mesh));
  }

  protected abstract buildMeshes(): Mesh[];
  protected abstract update(): void;
}

export default ThreeBaseObject;

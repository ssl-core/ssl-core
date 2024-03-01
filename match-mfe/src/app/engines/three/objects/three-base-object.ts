import { Group, Mesh } from "three";

abstract class ThreeBaseObject<T extends object = {}> extends Group {
  protected metadata: T;

  constructor(metadata: T = {} as T) {
    super();

    this.metadata = metadata;
    this.buildMeshes().forEach((mesh) => this.add(mesh));
  }

  protected abstract buildMeshes(): Mesh[];
  protected abstract update(): void;
}

export default ThreeBaseObject;

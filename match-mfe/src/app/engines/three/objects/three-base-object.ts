import { Group, Mesh } from "three";

abstract class ThreeBaseObject extends Group {
  constructor() {
    super();
    this.buildMeshes().forEach((mesh) => this.add(mesh));
  }

  protected abstract buildMeshes(): Mesh[];
  protected abstract update(): void;
}

export default ThreeBaseObject;

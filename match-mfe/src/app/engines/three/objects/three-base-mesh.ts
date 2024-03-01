import { BufferGeometry, Material, Mesh } from "three";

abstract class ThreeBaseMesh<T extends object = {}> extends Mesh {
  protected metadata: T;

  constructor(metadata: T = {} as T) {
    super();

    this.metadata = metadata;
    this.geometry = this.buildGeometry();
    this.material = this.buildMaterial();
  }

  protected abstract buildGeometry(): BufferGeometry;
  protected abstract buildMaterial(): Material;
}

export default ThreeBaseMesh;

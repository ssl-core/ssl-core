import { BufferGeometry, Material, Mesh } from "three";

abstract class ThreeBaseMesh extends Mesh {
  protected buildMesh() {
    this.geometry = this.buildGeometry();
    this.material = this.buildMaterial();
  }

  protected abstract buildGeometry(): BufferGeometry;
  protected abstract buildMaterial(): Material;
}

export default ThreeBaseMesh;

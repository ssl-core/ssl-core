import ThreeBaseObject from "../three-base-object";
import ThreeFieldSurfaceMesh from "./meshes/three-field-surface-mesh";
import ThreeFieldMesh from "./meshes/three-field-mesh";
import ThreeFieldLinesMesh from "./meshes/three-field-lines-mesh";

class ThreeFieldObject extends ThreeBaseObject {
  public update() {}

  protected buildMeshes() {
    const lines = new ThreeFieldLinesMesh();
    const field = new ThreeFieldMesh();
    const surface = new ThreeFieldSurfaceMesh();

    return [lines, field, surface];
  }
}

export default ThreeFieldObject;

import ThreeBaseObject from "../three-base-object";
import ThreeFieldSurfaceMesh from "./meshes/three-field-surface-mesh";
import ThreeFieldMesh from "./meshes/three-field-mesh";
import ThreeFieldLinesMesh from "./meshes/three-field-lines-mesh";

class ThreeFieldObject extends ThreeBaseObject {
  public update() {}

  protected buildMeshes() {
    const surface = new ThreeFieldSurfaceMesh();
    const field = new ThreeFieldMesh();
    const lines = new ThreeFieldLinesMesh();
    lines.scale.set(1, 1, 1);
    lines.computeLineDistances();

    return [surface, field, lines];
  }
}

export default ThreeFieldObject;

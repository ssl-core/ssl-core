import ThreeBaseObject from "../three-base-object";
import ThreeFieldSurfaceMesh from "../../meshes/field/three-field-surface-mesh";
import ThreeFieldMesh from "../../meshes/field/three-field-mesh";
import ThreeFieldLinesMesh from "../../meshes/field/three-field-lines-mesh";

class ThreeFieldObject extends ThreeBaseObject {
  constructor() {
    super();
    this.addMeshes();
  }

  public update() {}

  protected addMeshes() {
    const lines = new ThreeFieldLinesMesh();
    const field = new ThreeFieldMesh();
    const surface = new ThreeFieldSurfaceMesh();
    this.add(lines, field, surface);
  }
}

export default ThreeFieldObject;

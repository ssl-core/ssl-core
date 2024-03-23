import ThreeBaseObject from "../three-base-object";
import ThreeBallMesh from "../../meshes/ball/three-ball-mesh";

class ThreeBallObject extends ThreeBaseObject {
  constructor() {
    super();
    this.addMeshes();
  }

  public update() {}

  protected addMeshes() {
    const ball = new ThreeBallMesh();
    this.add(ball);
  }
}

export default ThreeBallObject;

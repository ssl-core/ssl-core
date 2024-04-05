import ThreeBaseObject from "../three-base-object";
import ThreeBallMesh from "../../meshes/ball/three-ball-mesh";

class ThreeBallObject extends ThreeBaseObject {
  constructor() {
    super();
    this.addMeshes();
  }

  public update() {}

  public setParams(params: Ball) {
    this.position.set(
      params.position[0],
      params.position[1],
      params.position[2]
    );
  }

  protected addMeshes() {
    const ball = new ThreeBallMesh();
    this.add(ball);
  }
}

export default ThreeBallObject;

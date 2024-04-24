import { CircleGeometry, MeshPhongMaterial } from "three";

import ThreeBaseMesh from "../three-base-mesh";
import constants from "../../../../../config/constants";

class ThreeDotMesh extends ThreeBaseMesh {
  private radius: number;
  private color: string;

  constructor(radius: number, color: string) {
    super();
    this.radius = radius;
    this.color = color;

    this.buildMesh();
  }

  public setColor(color: string) {
    this.color = color;
    this.material = this.buildMaterial();
  }

  protected buildGeometry() {
    return new CircleGeometry(
      this.radius,
      constants.robot.wheels.segments
    ).translate(0, 0, constants.epsilon);
  }

  protected buildMaterial() {
    return new MeshPhongMaterial({
      color: this.color,
      fog: false,
    });
  }
}

export default ThreeDotMesh;

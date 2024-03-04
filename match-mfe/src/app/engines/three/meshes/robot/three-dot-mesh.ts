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

  protected buildGeometry() {
    return new CircleGeometry(
      this.radius,
      constants.robot.wheels.segments
    ).translate(0, 0, 0.001);
  }

  protected buildMaterial() {
    return new MeshPhongMaterial({
      color: this.color,
    });
  }
}

export default ThreeDotMesh;

import { MeshPhongMaterial, SphereGeometry } from "three";

import ThreeBaseMesh from "../three-base-mesh";
import constants from "../../../../../config/constants";

class ThreeBallMesh extends ThreeBaseMesh {
  constructor() {
    super();
    this.castShadow = true;

    this.buildMesh();
  }

  protected buildGeometry() {
    return new SphereGeometry(
      constants.ball.radius,
      constants.ball.segments,
      constants.ball.segments
    ).translate(0, 0, constants.ball.radius);
  }

  protected buildMaterial() {
    return new MeshPhongMaterial({ color: constants.ball.color, fog: false });
  }
}

export default ThreeBallMesh;

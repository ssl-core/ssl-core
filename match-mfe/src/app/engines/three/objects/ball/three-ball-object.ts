import { Mesh, MeshPhongMaterial, SphereGeometry } from "three";

import ThreeBaseObject from "../three-base-object";
import constants from "../../../../../config/constants";

class ThreeBallObject extends ThreeBaseObject {
  public update() {}

  protected buildMeshes() {
    const ballGeometry = new SphereGeometry(
      constants.ball.radius,
      constants.ball.segments,
      constants.ball.segments
    ).translate(0, 0, -constants.ball.radius);
    const ballMaterial = new MeshPhongMaterial({ color: constants.ball.color });
    const ball = new Mesh(ballGeometry, ballMaterial);

    return [ball];
  }
}

export default ThreeBallObject;

import { CircleGeometry, MeshLambertMaterial } from "three";

import ThreeBaseMesh from "../three-base-mesh";
import constants from "../../../../../config/constants";

class ThreeRobotSelectionMesh extends ThreeBaseMesh {
  constructor() {
    super();
    this.buildMesh();
  }

  protected buildGeometry() {
    return new CircleGeometry(
      constants.robot.selection.radius,
      constants.robot.selection.segments
    ).translate(
      0,
      0,
      -constants.robot.chassis.height / 2 -
        constants.robot.chassis.bottomHeight +
        constants.epsilon
    );
  }

  protected buildMaterial() {
    return new MeshLambertMaterial({
      color: constants.robot.selection.color,
      opacity: constants.robot.selection.opacity,
      transparent: true,
      fog: false,
    });
  }
}

export default ThreeRobotSelectionMesh;

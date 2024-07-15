import { CylinderGeometry, MeshPhongMaterial } from "three";

import ThreeBaseMesh from "../three-base-mesh";
import constants from "../../../../../config/constants";

class ThreeChassisMesh extends ThreeBaseMesh {
  constructor() {
    super();
    this.castShadow = true;

    this.buildMesh();
  }

  protected buildGeometry() {
    return new CylinderGeometry(
      constants.robot.chassis.radius,
      constants.robot.chassis.radius,
      constants.robot.chassis.height,
      constants.robot.chassis.segments
    ).rotateX(Math.PI / 2);
  }

  protected buildMaterial() {
    return new MeshPhongMaterial({
      color: constants.robot.chassis.color,
      shininess: 10,
      fog: false,
    });
  }
}

export default ThreeChassisMesh;

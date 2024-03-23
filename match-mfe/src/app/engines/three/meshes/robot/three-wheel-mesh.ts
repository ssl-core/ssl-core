import { CylinderGeometry, MeshPhongMaterial } from "three";

import ThreeBaseMesh from "../three-base-mesh";
import constants from "../../../../../config/constants";
import { degreesToRadians } from "../../../../../utils/math";

class ThreeWheelMesh extends ThreeBaseMesh {
  constructor(angle: number) {
    super();
    this.setParamsByAngle(angle);
    this.buildMesh();
  }

  protected buildGeometry() {
    const geometry = new CylinderGeometry(
      constants.robot.wheels.radius,
      constants.robot.wheels.radius,
      constants.robot.wheels.thickness,
      constants.robot.wheels.segments
    );

    return geometry;
  }

  protected buildMaterial() {
    return new MeshPhongMaterial({
      color: constants.robot.wheels.color,
    });
  }

  private setParamsByAngle(angle: number) {
    const radius =
      constants.robot.chassis.radius - constants.robot.wheels.thickness / 2;
    const positionAngle = degreesToRadians(angle);
    const x = radius * Math.cos(positionAngle);
    const y = radius * Math.sin(positionAngle);
    const z =
      constants.robot.chassis.height / 2 -
      constants.robot.wheels.radius +
      constants.robot.chassis.bottomHeight;
    const rotationAngle = positionAngle + Math.PI / 2;

    this.position.set(x, y, -z);
    this.rotation.set(0, 0, rotationAngle);
  }
}

export default ThreeWheelMesh;

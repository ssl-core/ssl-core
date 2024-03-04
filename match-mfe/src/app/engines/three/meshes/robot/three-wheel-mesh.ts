import { CylinderGeometry, MeshPhongMaterial } from "three";

import ThreeBaseMesh from "../three-base-mesh";
import constants from "../../../../../config/constants";

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
      constants.robot.chassis.radius - constants.robot.wheels.thickness / 2.0;
    const positionAngle = (angle * Math.PI) / 180.0;
    const x = radius * Math.cos(positionAngle);
    const y = radius * Math.sin(positionAngle);
    const z =
      constants.robot.chassis.height / 2.0 -
      constants.robot.wheels.radius +
      constants.robot.chassis.bottomHeight;
    const rotationAngle = (angle * Math.PI) / 180.0 + Math.PI / 2.0;

    this.position.set(x, y, -z);
    this.rotation.set(0, 0, rotationAngle);
  }
}

export default ThreeWheelMesh;

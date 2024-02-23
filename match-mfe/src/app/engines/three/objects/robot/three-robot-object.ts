import { Vector3 } from "three";

import ThreeBaseObject from "../three-base-object";
import ThreeChassisMesh from "./meshes/three-chassis-mesh";
import ThreeWheelMesh from "./meshes/three-wheel-mesh";
import constants from "../../../../../config/constants";
import ThreeDotMesh from "./meshes/three-dot-mesh";

class ThreeRobotObject extends ThreeBaseObject {
  public update() {
    this.rotateOnWorldAxis(new Vector3(0, 1, 0.3), 0.01);
  }

  protected buildMeshes() {
    const chassis = new ThreeChassisMesh();
    chassis.position.set(0, 0, 0);

    const dotColors = this.getDotsColors();

    const centerDot = new ThreeDotMesh({
      color: dotColors.center,
      radius: constants.robot.dots.center.radius,
    });
    centerDot.position.set(0, 0, constants.robot.dots.common.y);

    const dot1 = new ThreeDotMesh({
      color: dotColors.other[0],
      radius: constants.robot.dots.other.radius,
    });
    dot1.position.set(
      constants.robot.dots.other.min_distance,
      constants.robot.dots.other.max_distance,
      constants.robot.dots.common.y
    );

    const dot2 = new ThreeDotMesh({
      color: dotColors.other[1],
      radius: constants.robot.dots.other.radius,
    });
    dot2.position.set(
      constants.robot.dots.other.min_distance,
      -constants.robot.dots.other.max_distance,
      constants.robot.dots.common.y
    );

    const dot3 = new ThreeDotMesh({
      color: dotColors.other[2],
      radius: constants.robot.dots.other.radius,
    });
    dot3.position.set(
      -constants.robot.dots.other.max_distance,
      constants.robot.dots.other.min_distance,
      constants.robot.dots.common.y
    );

    const dot4 = new ThreeDotMesh({
      color: dotColors.other[3],
      radius: constants.robot.dots.other.radius,
    });
    dot4.position.set(
      -constants.robot.dots.other.max_distance,
      -constants.robot.dots.other.min_distance,
      constants.robot.dots.common.y
    );

    const wheel1 = new ThreeWheelMesh();
    wheel1.setParamsByAngle(constants.robot.wheels.angles[0]);

    const wheel2 = new ThreeWheelMesh();
    wheel2.setParamsByAngle(constants.robot.wheels.angles[1]);

    const wheel3 = new ThreeWheelMesh();
    wheel3.setParamsByAngle(constants.robot.wheels.angles[2]);

    const wheel4 = new ThreeWheelMesh();
    wheel4.setParamsByAngle(constants.robot.wheels.angles[3]);

    return [
      chassis,
      centerDot,
      dot1,
      dot2,
      dot3,
      dot4,
      wheel1,
      wheel2,
      wheel3,
      wheel4,
    ];
  }

  private getDotsColors() {
    return {
      center: "#0000ff",
      other: ["#ff00ff", "#00ff00", "#ff0000", "#ffff00"],
    };
  }
}

export default ThreeRobotObject;

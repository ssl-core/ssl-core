import ThreeBaseObject from "../three-base-object";
import ThreeChassisMesh from "./meshes/three-chassis-mesh";
import ThreeWheelMesh from "./meshes/three-wheel-mesh";
import constants from "../../../../../config/constants";
import ThreeDotMesh from "./meshes/three-dot-mesh";

type ThreeRobotObjectMetadata = {
  robotId: number;
  team: "blue" | "yellow";
};

class ThreeRobotObject extends ThreeBaseObject<ThreeRobotObjectMetadata> {
  public update() {}

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
    const robotId: number = this.metadata?.robotId || 0;
    const team: "blue" | "yellow" = this.metadata?.team || "blue";

    return {
      center: constants.robot.dots.colors[team],
      other: constants.robot.dots.colors.patterns[robotId],
    };
  }
}

export default ThreeRobotObject;

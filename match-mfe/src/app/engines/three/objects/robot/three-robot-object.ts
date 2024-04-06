import ThreeBaseObject from "../three-base-object";
import ThreeChassisMesh from "../../meshes/robot/three-chassis-mesh";
import ThreeWheelMesh from "../../meshes/robot/three-wheel-mesh";
import ThreeDotMesh from "../../meshes/robot/three-dot-mesh";
import constants from "../../../../../config/constants";
import { degreesToRadians } from "../../../../../utils/math";

class ThreeRobotObject extends ThreeBaseObject {
  private robotId: number;
  private robotColor: RobotColor;

  constructor(robotId?: number, robotColor?: RobotColor) {
    super();
    this.robotId = robotId || 0;
    this.robotColor = robotColor || "yellow";

    this.addMeshes();
  }

  public update() {}

  public setParams(params: Robot) {
    this.position.set(
      params.position[0],
      params.position[1],
      constants.robot.chassis.height / 2 + constants.robot.chassis.bottomHeight
    );
    this.rotation.set(0, 0, degreesToRadians(params.angle));
  }

  public getRobotId() {
    return this.robotId;
  }

  public getRobotColor() {
    return this.robotColor;
  }

  protected addMeshes() {
    const chassis = new ThreeChassisMesh();
    chassis.position.set(0, 0, 0);
    this.add(chassis);

    const dotParams = this.getDotsParams();
    for (const dotParam of dotParams) {
      const dot = new ThreeDotMesh(dotParam.radius, dotParam.robotColor);
      dot.position.set(
        dotParam.position[0],
        dotParam.position[1],
        dotParam.position[2]
      );

      this.add(dot);
    }

    const angles = constants.robot.wheels.angles;
    for (const angle of angles) {
      const wheel = new ThreeWheelMesh(angle);
      this.add(wheel);
    }
  }

  private getDotsParams() {
    const robotId = this.robotId;
    const robotColor = this.robotColor;
    const robotColors = constants.robot.dots.colors;
    const centerRadius = constants.robot.dots.center.radius;
    const {
      minDistance,
      maxDistance,
      radius: otherRadius,
    } = constants.robot.dots.other;

    return [
      {
        radius: centerRadius,
        position: [0, 0, constants.robot.dots.common.z],
        robotColor: robotColors[robotColor],
      },
      {
        radius: otherRadius,
        position: [minDistance, maxDistance, constants.robot.dots.common.z],
        robotColor: robotColors.patterns[robotId][0],
      },
      {
        radius: otherRadius,
        position: [minDistance, -maxDistance, constants.robot.dots.common.z],
        robotColor: robotColors.patterns[robotId][1],
      },
      {
        radius: otherRadius,
        position: [-maxDistance, minDistance, constants.robot.dots.common.z],
        robotColor: robotColors.patterns[robotId][2],
      },
      {
        radius: otherRadius,
        position: [-maxDistance, -minDistance, constants.robot.dots.common.z],
        robotColor: robotColors.patterns[robotId][3],
      },
    ];
  }
}

export default ThreeRobotObject;

import ThreeBaseObject from "../three-base-object";
import ThreeChassisMesh from "../../meshes/robot/three-chassis-mesh";
import ThreeWheelMesh from "../../meshes/robot/three-wheel-mesh";
import ThreeDotMesh from "../../meshes/robot/three-dot-mesh";
import ThreeRobotSelectionMesh from "../../meshes/robot/three-robot-selection-mesh";
import constants from "../../../../../config/constants";
import { degreesToRadians } from "../../../../../utils/math";

class ThreeRobotObject extends ThreeBaseObject {
  private params: Robot | null;
  private robotId: number;
  private robotColor: RobotColor;
  private dots: ThreeDotMesh[];
  private selectionMesh: ThreeRobotSelectionMesh | null;

  constructor(robotId?: number, robotColor?: RobotColor) {
    super();
    this.selectable = true;
    this.params = null;
    this.robotId = robotId || 0;
    this.robotColor = robotColor || "yellow";
    this.dots = [];
    this.selectionMesh = null;

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

    if (this.needsToRedraw(params)) {
      this.robotId = params.robot_id;
      this.robotColor = params.robot_color;

      const dotParams = this.getDotsParams();
      for (const [index, dotParam] of dotParams.entries()) {
        const dot = this.dots[index];
        dot.setColor(dotParam.robotColor);
      }
    }

    this.params = params;
  }

  public getRobotId() {
    return this.robotId;
  }

  public getRobotColor() {
    return this.robotColor;
  }

  public onSelect() {
    if (!this.selectionMesh) {
      return;
    }

    this.selectionMesh.visible = true;
  }

  public onDeselect() {
    if (!this.selectionMesh) {
      return;
    }

    this.selectionMesh.visible = false;
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
      this.dots.push(dot);
    }

    const angles = constants.robot.wheels.angles;
    for (const angle of angles) {
      const wheel = new ThreeWheelMesh(angle);
      this.add(wheel);
    }

    this.selectionMesh = new ThreeRobotSelectionMesh();
    this.selectionMesh.position.set(0, 0, 0);
    this.selectionMesh.visible = false;
    this.add(this.selectionMesh);
  }

  private needsToRedraw(params: Robot) {
    return (
      !this.params ||
      this.params.robot_id !== params.robot_id ||
      this.params.robot_color !== params.robot_color
    );
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

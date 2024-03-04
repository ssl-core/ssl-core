import ThreeBaseObject from "../three-base-object";
import ThreeChassisMesh from "../../meshes/robot/three-chassis-mesh";
import ThreeWheelMesh from "../../meshes/robot/three-wheel-mesh";
import ThreeDotMesh from "../../meshes/robot/three-dot-mesh";
import constants from "../../../../../config/constants";

class ThreeRobotObject extends ThreeBaseObject {
  private robotId: number;
  private team: TeamColor;

  constructor(robotId: number, team: TeamColor) {
    super();
    this.robotId = robotId;
    this.team = team;

    this.addMeshes();
  }

  public update() {}

  protected addMeshes() {
    const chassis = new ThreeChassisMesh();
    chassis.position.set(0, 0, 0);
    this.add(chassis);

    const dotParams = this.getDotsParams();
    for (let i = 0; i < dotParams.length; i++) {
      const dot = new ThreeDotMesh(dotParams[i].radius, dotParams[i].color);
      const position = dotParams[i].position;
      dot.position.set(position[0], position[1], position[2]);

      this.add(dot);
    }

    const angles = constants.robot.wheels.angles;
    for (let i = 0; i < angles.length; i++) {
      const wheel = new ThreeWheelMesh(angles[i]);
      this.add(wheel);
    }
  }

  private getDotsParams() {
    const robotId = this.robotId;
    const team = this.team;
    const colors = constants.robot.dots.colors;
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
        color: colors[team],
      },
      {
        radius: otherRadius,
        position: [minDistance, maxDistance, constants.robot.dots.common.z],
        color: colors.patterns[robotId][0],
      },
      {
        radius: otherRadius,
        position: [minDistance, -maxDistance, constants.robot.dots.common.z],
        color: colors.patterns[robotId][1],
      },
      {
        radius: otherRadius,
        position: [-maxDistance, minDistance, constants.robot.dots.common.z],
        color: colors.patterns[robotId][2],
      },
      {
        radius: otherRadius,
        position: [-maxDistance, -minDistance, constants.robot.dots.common.z],
        color: colors.patterns[robotId][3],
      },
    ];
  }
}

export default ThreeRobotObject;

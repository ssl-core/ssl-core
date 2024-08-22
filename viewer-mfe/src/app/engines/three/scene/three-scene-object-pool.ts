import { Scene } from "three";

import ThreeFieldObject from "../objects/field/three-field-object";
import ThreeRobotObject from "../objects/robot/three-robot-object";
import ThreeBallObject from "../objects/ball/three-ball-object";

class ThreeSceneObjectPool {
  private scene: Scene;
  private field: ThreeFieldObject;
  private robots: ThreeRobotObject[];
  private balls: ThreeBallObject[];

  constructor(scene: Scene, maxRobots: number, maxBalls: number) {
    this.scene = scene;

    this.field = new ThreeFieldObject();
    this.field.visible = false;
    this.scene.add(this.field);

    this.robots = new Array(maxRobots).fill(null).map(() => {
      const robot = new ThreeRobotObject();
      robot.visible = false;
      this.scene.add(robot);

      return robot;
    });

    this.balls = new Array(maxBalls).fill(null).map(() => {
      const ball = new ThreeBallObject();
      ball.visible = false;
      this.scene.add(ball);

      return ball;
    });
  }

  public getField() {
    this.field.visible = true;
    return this.field;
  }

  public resetField() {
    this.field.visible = false;
  }

  public getRobot(id: number, color: RobotColor) {
    const robot = this.robots.find(
      (robot) =>
        robot.getRobotId() === id &&
        robot.getRobotColor() === color &&
        robot.visible
    );

    if (robot) {
      return robot;
    }

    const availableRobot = this.robots.find((robot) => !robot.visible);

    if (!availableRobot) {
      return null;
    }

    availableRobot.visible = true;
    return availableRobot;
  }

  public resetRobots() {
    for (const robot of this.robots) {
      robot.visible = false;
    }
  }

  public getBall(index: number) {
    if (index > this.balls.length || index < 0) {
      return null;
    }

    const ball = this.balls[index];
    ball.visible = true;
    return ball;
  }

  public resetBalls() {
    for (const ball of this.balls) {
      ball.visible = false;
    }
  }

  public getObjects() {
    return [this.field, ...this.robots, ...this.balls];
  }

  public getSelectableObjects() {
    return this.getObjects().filter((object) => object.isSelectable());
  }
}

export default ThreeSceneObjectPool;

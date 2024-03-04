import {
  Color,
  HemisphereLight,
  DirectionalLight,
  PerspectiveCamera,
  Scene,
  WebGLRenderer,
} from "three";
import { OrbitControls } from "three/examples/jsm/Addons.js";

import ThreeRobotObject from "../objects/robot/three-robot-object";
import ThreeFieldObject from "../objects/field/three-field-object";
import ThreeElementProxyReceiver from "../proxy/three-element-proxy-receiver";
import ThreeGoalObject from "../objects/goal/three-goal-object";
import ThreeBallObject from "../objects/ball/three-ball-object";
import ThreeWallsObject from "../objects/walls/three-walls-object";
import constants from "../../../../config/constants";

class ThreeSceneManager {
  private canvas: OffscreenCanvas | null;
  private canvasDOM: ThreeElementProxyReceiver | null;
  private renderer: WebGLRenderer | null;
  private camera: PerspectiveCamera;
  private scene: Scene;

  constructor() {
    this.canvas = null;
    this.canvasDOM = null;
    this.renderer = null;
    this.camera = new PerspectiveCamera();
    this.scene = new Scene();
  }

  public initialize(
    canvas: OffscreenCanvas,
    canvasDOM: ThreeElementProxyReceiver
  ) {
    this.setCanvas(canvas, canvasDOM);
    this.setCameraPosition();
    this.setOrbitControls();
    this.addObjects();
    this.resize(canvas.width, canvas.height);
  }

  public render(_frame: Frame) {
    this.scene.children.forEach((child) => {
      if (child instanceof ThreeRobotObject) {
        child.update();
      }
    });

    this.update();
  }

  private update() {
    if (!this.renderer) {
      throw new Error("Renderer not initialized");
    }

    this.renderer.render(this.scene, this.camera);
  }

  public resize(width: number, height: number) {
    if (!this.canvas || !this.renderer) {
      throw new Error("Renderer not initialized");
    }

    this.camera.aspect = width / height;
    this.camera.updateProjectionMatrix();
    this.renderer.setSize(width, height, false);
    this.canvasDOM?.setSize({ top: 0, left: 0, width, height });
    this.update();
  }

  private addObjects() {
    this.addLight();
    this.addField();
    this.addRobots();
    this.addBall();
  }

  private addLight() {
    this.scene.background = new Color(constants.background);

    const directionalLight = new DirectionalLight("#FFFFFF", 8);
    directionalLight.position.set(-1, 2, 4).normalize();
    directionalLight.lookAt(0, 0, 0);
    this.scene.add(directionalLight);

    const light = new HemisphereLight("#FFFFFF", "#080808", 4.5);
    light.position.set(-1, 2, 4);
    this.scene.add(light);
  }

  private addField() {
    const field = new ThreeFieldObject();
    field.position.set(0, 0, 0);
    this.scene.add(field);

    const blueGoal = new ThreeGoalObject();
    blueGoal.position.set(
      -(constants.field.width + constants.goal.depth) / 2,
      0,
      constants.goal.height / 2
    );
    blueGoal.rotation.set(0, Math.PI, 0);
    this.scene.add(blueGoal);

    const yellowGoal = new ThreeGoalObject();
    yellowGoal.position.set(
      (constants.field.width + constants.goal.depth) / 2,
      0,
      constants.goal.height / 2
    );
    this.scene.add(yellowGoal);

    const walls = new ThreeWallsObject();
    walls.position.set(0, 0, constants.wall.height / 2);
    this.scene.add(walls);
  }

  private addRobots() {
    const z =
      constants.robot.chassis.height / 2.0 +
      constants.robot.chassis.bottomHeight;

    for (let i = 0; i < 11; i++) {
      const robot = new ThreeRobotObject(i, "blue");

      if (i === 0) {
        robot.position.set(-constants.field.width / 2, 0, z);
      } else if (i < 6) {
        robot.position.set(-3.5, (i - 3) * 1.5, z);
      } else {
        robot.position.set(-1.5, i - 8, z);
      }

      this.scene.add(robot);
    }

    for (let i = 0; i < 11; i++) {
      const robot = new ThreeRobotObject(i, "yellow");

      if (i === 0) {
        robot.position.set(constants.field.width / 2, 0, z);
      } else if (i < 6) {
        robot.position.set(3.5, (i - 3) * 1.5, z);
      } else {
        robot.position.set(1.5, i - 8, z);
      }

      this.scene.add(robot);
    }
  }

  private addBall() {
    const ball = new ThreeBallObject();
    ball.position.set(0, 0, constants.ball.radius);
    this.scene.add(ball);
  }

  private setCameraPosition() {
    this.camera.position.set(0, 0, 12);
    this.camera.lookAt(0, 0, 0);
  }

  private setCanvas(
    canvas: OffscreenCanvas,
    canvasDOM: ThreeElementProxyReceiver
  ) {
    this.canvas = canvas;
    this.canvasDOM = canvasDOM;
    this.renderer = new WebGLRenderer({
      canvas,
      antialias: true,
    });
  }

  private setOrbitControls() {
    if (!this.renderer || !this.canvasDOM) {
      throw new Error("Elements not initialized");
    }

    // @ts-ignore
    const controls = new OrbitControls(this.camera, this.canvasDOM);
    controls.target.set(0, 0, 0);
    controls.update();
  }
}

export default ThreeSceneManager;

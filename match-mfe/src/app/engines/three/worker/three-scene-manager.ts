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
import ElementProxyReceiver from "./three-element-proxy-receiver";
import ThreeGoalObject from "../objects/goal/three-goal-object";
import constants from "../../../../config/constants";
import ThreeBallObject from "../objects/ball/three-ball-object";

class ThreeSceneManager {
  private canvas: OffscreenCanvas | null;
  private proxy: ElementProxyReceiver | null;
  private renderer: WebGLRenderer | null;
  private camera: PerspectiveCamera;
  private scene: Scene;

  constructor() {
    this.canvas = null;
    this.proxy = null;
    this.renderer = null;
    this.camera = new PerspectiveCamera();
    this.scene = new Scene();
  }

  public initialize(canvas: OffscreenCanvas, proxy: ElementProxyReceiver) {
    this.setCanvas(canvas);
    this.setProxy(proxy);
    this.addLight();
    this.addObjects();
    this.setCameraPosition();
    this.setOrbitControls();
    this.resize(canvas.width, canvas.height);
  }

  public renderFrame(_match: Match) {
    this.scene.children.forEach((child) => {
      if (child instanceof ThreeRobotObject) {
        child.update();
      }
    });

    this.update();
  }

  public resize(width: number, height: number) {
    if (!this.canvas || !this.renderer) {
      throw new Error("Renderer not initialized");
    }

    this.camera.aspect = width / height;
    this.camera.updateProjectionMatrix();
    this.renderer.setSize(width, height, false);
    this.proxy?.setSize({ top: 0, left: 0, width, height });
    this.update();
  }

  private update() {
    if (!this.renderer) {
      throw new Error("Renderer not initialized");
    }

    this.renderer.render(this.scene, this.camera);
  }

  private addObjects() {
    const robot = new ThreeRobotObject();
    robot.position.set(-2, 0, 0);
    this.scene.add(robot);

    const field = new ThreeFieldObject();
    field.position.set(0, 0, -0.1);
    this.scene.add(field);

    const goal1 = new ThreeGoalObject();
    goal1.position.set(-constants.field.width / 2, 0, 0);
    this.scene.add(goal1);

    const goal2 = new ThreeGoalObject();
    goal2.position.set(constants.field.width / 2, 0, 0);
    this.scene.add(goal2);

    const ball = new ThreeBallObject();
    ball.position.set(0, 0, 0);
    this.scene.add(ball);
  }

  private addLight() {
    this.scene.background = new Color(0x333333);

    const directionalLight = new DirectionalLight(0xffffff, 8);
    directionalLight.position.set(-1, 2, 4).normalize();
    directionalLight.lookAt(0, 0, 0);
    this.scene.add(directionalLight);

    const light = new HemisphereLight(0xffffff, 0x080808, 4.5);
    light.position.set(-1, 2, 4);
    this.scene.add(light);
  }

  private setCameraPosition() {
    this.camera.position.set(0, 0, 12);
    this.camera.lookAt(0, 0, 0);
  }

  private setCanvas(canvas: OffscreenCanvas) {
    this.canvas = canvas;
    this.renderer = new WebGLRenderer({
      canvas,
      antialias: true,
    });
  }

  private setProxy(proxy: ElementProxyReceiver) {
    this.proxy = proxy;
  }

  private setOrbitControls() {
    if (!this.renderer || !this.proxy) {
      throw new Error("Elements not initialized");
    }

    // @ts-ignore
    const controls = new OrbitControls(this.camera, this.proxy);
    controls.target.set(0, 0, 0);
    controls.update();
  }
}

export default ThreeSceneManager;

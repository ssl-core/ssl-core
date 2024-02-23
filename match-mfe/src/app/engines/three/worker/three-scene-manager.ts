import {
  HemisphereLight,
  PCFSoftShadowMap,
  PerspectiveCamera,
  Scene,
  WebGLRenderer,
} from "three";

import ThreeRobotObject from "../objects/robot/three-robot-object";
import ThreeFieldObject from "../objects/field/three-field-object";

class ThreeSceneManager {
  private canvas: OffscreenCanvas | null;
  private renderer: WebGLRenderer | null;
  private camera: PerspectiveCamera;
  private scene: Scene;

  constructor() {
    this.canvas = null;
    this.renderer = null;
    this.camera = new PerspectiveCamera();
    this.scene = new Scene();
  }

  public initialize(canvas: OffscreenCanvas) {
    this.setCanvas(canvas);
    this.addLight();
    this.addObjects();
    this.setCameraPosition();
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

    this.canvas.width = width;
    this.canvas.height = height;
    this.camera.aspect = width / height;
    this.camera.updateProjectionMatrix();
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
    robot.position.set(0, 0, 0);
    this.scene.add(robot);

    const field = new ThreeFieldObject();
    field.position.set(0, 0, -0.1);
    this.scene.add(field);
  }

  private addLight() {
    const light = new HemisphereLight(0xffffff, 0x080808, 8);
    light.position.set(-1, 2, 4);
    this.scene.add(light);
  }

  private setCameraPosition() {
    this.camera.position.set(0, 0, 1);
    this.camera.lookAt(0, 0, 0);
  }

  private setCanvas(canvas: OffscreenCanvas) {
    this.canvas = canvas;
    this.renderer = new WebGLRenderer({
      canvas,
      antialias: true,
    });
    this.renderer.shadowMap.enabled = true;
    this.renderer.shadowMap.type = PCFSoftShadowMap;
  }
}

export default ThreeSceneManager;

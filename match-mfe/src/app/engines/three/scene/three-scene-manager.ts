import {
  Color,
  HemisphereLight,
  DirectionalLight,
  PerspectiveCamera,
  Scene,
  WebGLRenderer,
  Fog,
  GridHelper,
} from "three";
import { OrbitControls } from "three/examples/jsm/Addons.js";

import ThreeElementProxyReceiver from "../proxy/three-element-proxy-receiver";
import ThreeSceneObjectPool from "./three-scene-object-pool";
import Channels from "../../../../config/channels";
import constants from "../../../../config/constants";

class ThreeSceneManager {
  private canvas: OffscreenCanvas | null;
  private canvasDOM: ThreeElementProxyReceiver | null;
  private renderer: WebGLRenderer | null;
  private isRendering: boolean;
  private camera: PerspectiveCamera;
  private scene: Scene;
  private channel: BroadcastChannel;
  private pool: ThreeSceneObjectPool;

  constructor() {
    this.canvas = null;
    this.canvasDOM = null;
    this.renderer = null;
    this.isRendering = false;
    this.camera = new PerspectiveCamera();
    this.scene = new Scene();
    this.channel = new BroadcastChannel(Channels.Engine);
    this.pool = new ThreeSceneObjectPool(
      this.scene,
      constants.numRobots,
      constants.numBalls
    );
  }

  public initialize(
    canvas: OffscreenCanvas,
    canvasDOM: ThreeElementProxyReceiver
  ) {
    this.setCanvas(canvas, canvasDOM);
    this.setCameraPosition();
    this.setOrbitControls();
    this.setLighting();
    this.setGrid();
    this.listenChannel();
    this.resize(canvas.width, canvas.height);
  }

  public render(frame: Frame) {
    if (this.isRendering) {
      return;
    }

    this.isRendering = true;

    self.requestAnimationFrame(() => {
      const { field, robots, balls } = frame;

      this.renderField(field);
      this.renderRobots(robots);
      this.renderBalls(balls);
      this.update();
      this.notifyTester();
      this.isRendering = false;
    });
  }

  public tick() {
    for (const object of this.pool.getObjects()) {
      object.update();
    }

    this.update();
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

  private update() {
    if (!this.renderer) {
      throw new Error("Renderer not initialized");
    }

    this.renderer.render(this.scene, this.camera);
  }

  private setLighting() {
    this.scene.background = new Color(constants.background);

    const directionalLight = new DirectionalLight("#FFFFFF", 8);
    directionalLight.position.set(-1, 2, 4).normalize();
    directionalLight.lookAt(0, 0, 0);
    this.scene.add(directionalLight);

    const light = new HemisphereLight("#FFFFFF", "#080808", 4.5);
    light.position.set(-1, 2, 4);
    this.scene.add(light);
  }

  private setGrid() {
    const fog = new Fog(constants.background, 4, 40);
    this.scene.fog = fog;

    const grid = new GridHelper(200, 200, 0xffffff, 0xffffff);
    grid.material.opacity = 0.2;
    grid.material.transparent = true;
    grid.rotation.x = Math.PI / 2;
    grid.position.z = -constants.clippingEpsilon;
    this.scene.add(grid);
  }

  private renderField(fieldParams: Field) {
    const field = this.pool.getField();
    field.setParams(fieldParams);
  }

  private renderRobots(robotsParams: Robot[]) {
    for (const robotParams of robotsParams) {
      const robot = this.pool.getRobot(
        robotParams.robot_id,
        robotParams.robot_color
      );
      robot?.setParams(robotParams);
    }
  }

  private renderBalls(ballsParams: Ball[]) {
    for (const [index, ballParams] of ballsParams.entries()) {
      const ball = this.pool.getBall(index);
      ball?.setParams(ballParams);
    }
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

  private listenChannel() {
    this.channel.addEventListener("message", (event) => {
      this.render(event.data as Frame);
    });
  }

  private setOrbitControls() {
    if (!this.renderer || !this.canvasDOM) {
      throw new Error("Elements not initialized");
    }

    // @ts-ignore
    const controls = new OrbitControls(this.camera, this.canvasDOM);
    controls.target.set(0, 0, 0);
    controls.minDistance = 1;
    controls.maxDistance = 20;
    controls.enablePan = false;
    controls.update();
  }

  private notifyTester() {
    self.postMessage(performance.now());
  }
}

export default ThreeSceneManager;

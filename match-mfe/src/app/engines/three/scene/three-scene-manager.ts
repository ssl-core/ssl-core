import {
  Color,
  HemisphereLight,
  DirectionalLight,
  PerspectiveCamera,
  Scene,
  WebGLRenderer,
  Fog,
  GridHelper,
  Raycaster,
  Vector2,
  Vector3,
} from "three";
import { OrbitControls } from "three/examples/jsm/Addons.js";

import ThreeElementProxyReceiver from "../proxy/three-element-proxy-receiver";
import ThreeSceneObjectPool from "./three-scene-object-pool";
import ThreeBaseObject from "../objects/three-base-object";
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
  private raycaster: Raycaster;
  private orbitControls: OrbitControls | null;

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
    this.raycaster = new Raycaster();
    this.orbitControls = null;
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
    this.listenMouseEvents();
    this.listenChannel();
    this.resize(0, 0, canvas.width, canvas.height);
    this.loop();
  }

  public render(frame: Frame) {
    if (this.isRendering) {
      return;
    }

    const { field, robots, balls } = frame;

    this.renderField(field);
    this.renderRobots(robots);
    this.renderBalls(balls);
  }

  public tick() {
    for (const object of this.pool.getObjects()) {
      object.update();
    }

    this.update();
  }

  public resize(top: number, left: number, width: number, height: number) {
    if (!this.canvas || !this.renderer) {
      throw new Error("Renderer not initialized");
    }

    this.camera.aspect = width / height;
    this.camera.updateProjectionMatrix();
    this.renderer.setSize(width, height, false);
    this.canvasDOM?.setSize({ top, left, width, height });
  }

  public setAxis(axis: any) {
    if (!this.orbitControls) {
      throw new Error("OrbitControls not initialized");
    }

    let distance = this.camera.position.distanceTo(this.orbitControls.target);
    this.camera.position.copy(
      new Vector3(axis.direction.x, axis.direction.y, axis.direction.z)
        .multiplyScalar(distance)
        .add(this.orbitControls.target)
    );
    this.camera.lookAt(this.orbitControls.target);
  }

  private update() {
    if (!this.renderer) {
      throw new Error("Renderer not initialized");
    }

    this.renderer.render(this.scene, this.camera);
    this.sync();
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

  private listenMouseEvents() {
    if (!this.canvasDOM) {
      throw new Error("Canvas not initialized");
    }

    // @ts-ignore
    this.canvasDOM.addEventListener("mousedown", (event: MouseEvent) => {
      const canvasRect = this.canvasDOM!.getBoundingClientRect();

      const pointer = new Vector2(
        ((event.clientX - canvasRect?.left) / canvasRect?.width) * 2 - 1,
        -((event.clientY - canvasRect?.top) / canvasRect?.height) * 2 + 1
      );

      this.raycaster.setFromCamera(pointer, this.camera);

      const selectableObjects = this.pool.getSelectableObjects();
      const intersects = this.raycaster.intersectObjects(selectableObjects);

      for (const object of selectableObjects) {
        object.deselect();
      }

      if (intersects.length > 0) {
        const object = intersects[0].object.parent as ThreeBaseObject;

        if (!object.isSelectable()) {
          return;
        }

        object.select();

        this.orbitControls!.target = object.position;
        this.orbitControls!.update();
      }
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
    this.orbitControls = new OrbitControls(this.camera, this.canvasDOM);
    this.orbitControls.target.set(0, 0, 0);
    this.orbitControls.minDistance = 1;
    this.orbitControls.maxDistance = 20;
    this.orbitControls.enablePan = false;
    this.orbitControls.update();
  }

  private sync() {
    const syncMessage: ThreeSyncMessage = {
      camera: this.camera.rotation.toArray(),
      time: performance.now(),
    };

    self.postMessage(syncMessage);
  }

  private loop() {
    if (!this.renderer) {
      throw new Error("Renderer not initialized");
    }

    this.renderer.setAnimationLoop(() => {
      this.isRendering = true;
      this.tick();
      this.isRendering = false;
    });
  }
}

export default ThreeSceneManager;

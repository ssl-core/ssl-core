import {
  Color,
  DirectionalLight,
  PerspectiveCamera,
  Scene,
  WebGLRenderer,
  Fog,
  GridHelper,
  Raycaster,
  Vector2,
  Vector3,
  SRGBColorSpace,
  AmbientLight,
  PCFSoftShadowMap,
} from "three";

import ThreeElementProxyReceiver from "../proxy/three-element-proxy-receiver";
import ThreeSceneObjectPool from "./three-scene-object-pool";
import ThreeBaseObject from "../objects/three-base-object";
import Channels from "../../../../config/channels";
import constants from "../../../../config/constants";
import OrbitNavigation from "../lib/orbit-navigation";

class ThreeSceneManager {
  private canvas: OffscreenCanvas | null;
  private canvasDOM: ThreeElementProxyReceiver | null;
  private renderer: WebGLRenderer | null;
  private orbitNavigation: OrbitNavigation | null;
  private isRendering: boolean;
  private camera: PerspectiveCamera;
  private scene: Scene;
  private channel: BroadcastChannel;
  private pool: ThreeSceneObjectPool;
  private raycaster: Raycaster;

  constructor() {
    this.canvas = null;
    this.canvasDOM = null;
    this.renderer = null;
    this.orbitNavigation = null;
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
  }

  public initialize(
    canvas: OffscreenCanvas,
    canvasDOM: ThreeElementProxyReceiver
  ) {
    this.setCanvas(canvas, canvasDOM);
    this.setCameraPosition();
    this.setOrbitNavigation();
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
    if (!this.orbitNavigation) {
      throw new Error("Orbit navigation not initialized");
    }

    const target = this.orbitNavigation.getTarget();
    const distance = this.camera.position.distanceTo(target);
    this.camera.position.copy(
      new Vector3(axis.direction.x, axis.direction.y, axis.direction.z)
        .multiplyScalar(distance)
        .add(target)
    );
    this.camera.lookAt(target);
  }

  private update() {
    if (!this.renderer) {
      throw new Error("Renderer not initialized");
    }

    this.renderer.render(this.scene, this.camera);
    this.orbitNavigation?.update();
    this.sync();
  }

  private setLighting() {
    this.scene.background = new Color(constants.background);

    const ambientLight = new AmbientLight("#FFFFFF", 2);
    this.scene.add(ambientLight);

    const directionalLight = new DirectionalLight("#FFFFFF", 12);
    directionalLight.position.set(0, 0, 8).normalize();
    directionalLight.lookAt(0, 0, 0);
    directionalLight.castShadow = true;
    this.scene.add(directionalLight);
  }

  private setGrid() {
    const fog = new Fog(constants.background, 4, 36);
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
    this.camera.up.set(0, 0, 1);
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
      powerPreference: "high-performance",
      antialias: true,
      alpha: true,
      logarithmicDepthBuffer: true,
    });
    this.renderer.outputColorSpace = SRGBColorSpace;
    this.renderer.shadowMap.enabled = true;
    this.renderer.shadowMap.type = PCFSoftShadowMap;
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
      const intersected =
        intersects.length > 0 &&
        (intersects[0].object.parent as ThreeBaseObject);

      for (const object of selectableObjects) {
        object.deselect();
      }

      if (!this.orbitNavigation) {
        return;
      }

      if (intersected && intersected.isSelectable()) {
        intersected.select();
        this.orbitNavigation.follow(intersected);
      } else {
        this.orbitNavigation.unfollow();
      }
    });
  }

  private listenChannel() {
    this.channel.addEventListener("message", (event) => {
      this.render(event.data as Frame);
    });
  }

  private setOrbitNavigation() {
    if (!this.canvasDOM) {
      throw new Error("Elements not initialized");
    }

    this.orbitNavigation = new OrbitNavigation(this.camera, this.canvasDOM);
    this.orbitNavigation.initialize();
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

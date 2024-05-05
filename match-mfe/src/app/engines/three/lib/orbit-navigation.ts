import { Camera, Vector3 } from "three";
import { OrbitControls } from "three/examples/jsm/Addons.js";

import ThreeElementProxyReceiver from "../proxy/three-element-proxy-receiver";
import ThreeBaseObject from "../objects/three-base-object";

class OrbitNavigation {
  private static readonly FOLLOW_SPEED = 0.1;

  private orbitControls: OrbitControls;
  private followObject: ThreeBaseObject | null;

  constructor(camera: Camera, dom: ThreeElementProxyReceiver) {
    // @ts-ignore
    this.orbitControls = new OrbitControls(camera, dom);
    this.followObject = null;
  }

  public initialize() {
    this.orbitControls.target.set(0, 0, 0);
    this.orbitControls.minDistance = 1;
    this.orbitControls.maxDistance = 20;
    this.orbitControls.enablePan = false;
    this.orbitControls.update();
  }

  public getTarget() {
    return this.orbitControls.target;
  }

  public setTarget(position: Vector3) {
    const diff = position.clone().sub(this.orbitControls.target);
    this.orbitControls.object.position.add(diff);
    this.orbitControls.target = position;
  }

  public follow(object: ThreeBaseObject) {
    this.followObject = object;
  }

  public unfollow() {
    this.followObject = null;
  }

  public update() {
    if (this.followObject) {
      const newPosition = this.followObject.position.clone();
      const diff = newPosition.sub(this.orbitControls.target);
      diff.multiplyScalar(OrbitNavigation.FOLLOW_SPEED);
      this.setTarget(this.orbitControls.target.clone().add(diff));
    }

    this.orbitControls.update();
  }
}

export default OrbitNavigation;

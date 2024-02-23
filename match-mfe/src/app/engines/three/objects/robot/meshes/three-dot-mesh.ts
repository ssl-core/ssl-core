import { CircleGeometry, MeshPhongMaterial } from "three";

import ThreeBaseMesh from "../../three-base-mesh";
import constants from "../../../../../../config/constants";

class ThreeDotMesh extends ThreeBaseMesh {
  protected buildGeometry() {
    return new CircleGeometry(
      this.metadata?.radius,
      constants.robot.wheels.segments
    ).translate(0, 0, 0.001);
  }

  protected buildMaterial() {
    return new MeshPhongMaterial({
      color: this.metadata?.color,
    });
  }
}

export default ThreeDotMesh;

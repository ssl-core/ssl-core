import { MeshPhongMaterial, BoxGeometry } from "three";

import ThreeBaseMesh from "../three-base-mesh";
import constants from "../../../../../config/constants";

class ThreeWallMesh extends ThreeBaseMesh {
  private width: number;

  constructor(width: number) {
    super();
    this.width = width;

    this.buildMesh();
  }

  protected buildGeometry() {
    return new BoxGeometry(
      this.width,
      constants.wall.thickness,
      constants.wall.height
    );
  }

  protected buildMaterial() {
    return new MeshPhongMaterial({ color: constants.wall.color });
  }
}

export default ThreeWallMesh;

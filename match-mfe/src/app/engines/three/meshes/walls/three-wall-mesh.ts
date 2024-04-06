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

  public setWidth(width: number) {
    this.width = width;
    this.buildMesh();
  }

  protected buildGeometry() {
    return new BoxGeometry(
      this.width,
      constants.field.wall.thickness,
      constants.field.wall.height
    );
  }

  protected buildMaterial() {
    return new MeshPhongMaterial({ color: constants.field.wall.color });
  }
}

export default ThreeWallMesh;

import { MeshPhongMaterial, PlaneGeometry } from "three";

import ThreeBaseMesh from "../three-base-mesh";
import constants from "../../../../../config/constants";

class ThreeFieldSurfaceMesh extends ThreeBaseMesh {
  constructor() {
    super();
    this.buildMesh();
  }

  protected buildGeometry() {
    const width =
      constants.field.width +
      constants.field.outside.margin * 2 +
      constants.field.surface.margin * 2;
    const height =
      constants.field.height +
      constants.field.outside.margin * 2 +
      constants.field.surface.margin * 2;

    return new PlaneGeometry(width, height).translate(0, 0, -constants.epsilon);
  }

  protected buildMaterial() {
    return new MeshPhongMaterial({ color: constants.field.surface.color });
  }
}

export default ThreeFieldSurfaceMesh;
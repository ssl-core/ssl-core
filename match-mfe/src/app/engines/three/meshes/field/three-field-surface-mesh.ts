import { MeshPhongMaterial, PlaneGeometry } from "three";

import ThreeBaseMesh from "../three-base-mesh";
import constants from "../../../../../config/constants";

class ThreeFieldSurfaceMesh extends ThreeBaseMesh {
  protected buildGeometry() {
    return new PlaneGeometry(
      constants.field.width +
        constants.field.outside.margin * 2 +
        constants.field.surface.margin * 2,
      constants.field.height +
        constants.field.outside.margin * 2 +
        constants.field.surface.margin * 2
    ).translate(0, 0, -0.001);
  }

  protected buildMaterial() {
    return new MeshPhongMaterial({ color: constants.field.surface.color });
  }
}

export default ThreeFieldSurfaceMesh;

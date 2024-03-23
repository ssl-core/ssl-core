import { MeshLambertMaterial, PlaneGeometry } from "three";

import ThreeBaseMesh from "../three-base-mesh";
import constants from "../../../../../config/constants";

class ThreeFieldMesh extends ThreeBaseMesh {
  constructor() {
    super();
    this.buildMesh();
  }

  protected buildGeometry() {
    const width = constants.field.width + constants.field.outside.margin * 2;
    const height = constants.field.height + constants.field.outside.margin * 2;

    return new PlaneGeometry(width, height);
  }

  protected buildMaterial() {
    return new MeshLambertMaterial({ color: constants.field.color });
  }
}

export default ThreeFieldMesh;

import { MeshLambertMaterial, PlaneGeometry } from "three";

import ThreeBaseMesh from "../../three-base-mesh";
import constants from "../../../../../../config/constants";

class ThreeFieldMesh extends ThreeBaseMesh {
  protected buildGeometry() {
    return new PlaneGeometry(
      constants.field.width + constants.field.outside.margin * 2,
      constants.field.height + constants.field.outside.margin * 2
    );
  }

  protected buildMaterial() {
    return new MeshLambertMaterial({ color: constants.field.color });
  }
}

export default ThreeFieldMesh;

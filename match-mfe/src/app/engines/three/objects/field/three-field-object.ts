import { Mesh, MeshPhongMaterial, PlaneGeometry } from "three";

import ThreeBaseObject from "../three-base-object";
import constants from "../../../../../config/constants";

class ThreeFieldObject extends ThreeBaseObject {
  public update() {}

  protected buildMeshes() {
    const geometry = this.buildGeometry();
    const material = this.buildMaterial();
    const mesh = new Mesh(geometry, material);
    return [mesh];
  }

  private buildGeometry() {
    return new PlaneGeometry(
      constants.field.width,
      constants.field.height,
      constants.field.segments,
      constants.field.segments
    );
  }

  private buildMaterial() {
    return new MeshPhongMaterial({ color: constants.field.color });
  }
}

export default ThreeFieldObject;

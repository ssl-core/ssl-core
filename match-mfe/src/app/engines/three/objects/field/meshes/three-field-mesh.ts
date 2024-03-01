import {
  CanvasTexture,
  ImageBitmapLoader,
  MeshLambertMaterial,
  PlaneGeometry,
  RepeatWrapping,
} from "three";

import ThreeBaseMesh from "../../three-base-mesh";
import constants from "../../../../../../config/constants";
import grass from "../../../../../../assets/grasslight.jpg?url";

class ThreeFieldMesh extends ThreeBaseMesh {
  private static readonly TEXTURE_REPEAT_FACTOR = 0.5;

  protected buildGeometry() {
    return new PlaneGeometry(
      constants.field.width + constants.field.outside.margin * 2,
      constants.field.height + constants.field.outside.margin * 2
    );
  }

  protected buildMaterial() {
    this.loadTexture();
    return new MeshLambertMaterial({ color: constants.field.color });
  }

  private loadTexture() {
    const loader = new ImageBitmapLoader();

    loader.load(grass, (imageBitmap) => {
      const texture = new CanvasTexture(imageBitmap);
      texture.repeat.set(
        constants.field.width * ThreeFieldMesh.TEXTURE_REPEAT_FACTOR,
        constants.field.height * ThreeFieldMesh.TEXTURE_REPEAT_FACTOR
      );
      texture.wrapS = RepeatWrapping;
      texture.wrapT = RepeatWrapping;

      this.material = new MeshLambertMaterial({
        color: constants.field.color,
        map: texture,
      });
    });
  }
}

export default ThreeFieldMesh;

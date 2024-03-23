import ThreeBaseObject from "../three-base-object";
import constants from "../../../../../config/constants";
import ThreeWallMesh from "../../meshes/walls/three-wall-mesh";

class ThreeWallsObject extends ThreeBaseObject {
  constructor() {
    super();
    this.addMeshes();
  }

  public update() {}

  protected addMeshes() {
    const params = this.getWallsParams();
    for (let i = 0; i < params.length; i++) {
      const wall = new ThreeWallMesh(params[i].width);
      wall.position.set(
        params[i].position.x,
        params[i].position.y,
        params[i].position.z
      );
      wall.rotation.set(
        params[i].rotation.x,
        params[i].rotation.y,
        params[i].rotation.z
      );

      this.add(wall);
    }
  }

  private getWallsParams() {
    const width =
      constants.field.width +
      constants.field.outside.margin * 2 +
      constants.field.surface.margin * 2;
    const height =
      constants.field.height +
      constants.field.outside.margin * 2 +
      constants.field.surface.margin * 2;

    return [
      {
        width: width,
        position: { x: 0, y: -height / 2, z: 0 },
        rotation: { x: 0, y: 0, z: 0 },
      },
      {
        width: width,
        position: { x: 0, y: height / 2, z: 0 },
        rotation: { x: 0, y: 0, z: 0 },
      },
      {
        width: height,
        position: { x: width / 2, y: 0, z: 0 },
        rotation: { x: 0, y: 0, z: Math.PI / 2 },
      },
      {
        width: height,
        position: { x: -width / 2, y: 0, z: 0 },
        rotation: { x: 0, y: 0, z: Math.PI / 2 },
      },
    ];
  }
}

export default ThreeWallsObject;

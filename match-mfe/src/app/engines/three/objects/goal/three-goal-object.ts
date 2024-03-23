import { BoxGeometry, Mesh, MeshPhongMaterial } from "three";

import ThreeBaseObject from "../three-base-object";
import constants from "../../../../../config/constants";

class ThreeGoalObject extends ThreeBaseObject {
  constructor() {
    super();
    this.addMeshes();
  }

  public update() {}

  protected addMeshes() {
    const params = this.getWallsParams();

    for (const param of params) {
      const wall = this.buildMesh(param.width);
      wall.position.set(param.position.x, param.position.y, param.position.z);
      wall.rotation.set(param.rotation.x, param.rotation.y, param.rotation.z);

      this.add(wall);
    }
  }

  private buildMesh(width: number) {
    const geometry = new BoxGeometry(
      width,
      constants.wall.thickness,
      constants.wall.height
    );
    const material = new MeshPhongMaterial({ color: constants.goal.color });
    const wall = new Mesh(geometry, material);

    return wall;
  }

  private getWallsParams() {
    return [
      {
        width: constants.goal.depth,
        position: { x: 0, y: -constants.goal.width / 2, z: 0 },
        rotation: { x: 0, y: 0, z: 0 },
      },
      {
        width: constants.goal.depth,
        position: { x: 0, y: constants.goal.width / 2, z: 0 },
        rotation: { x: 0, y: 0, z: 0 },
      },
      {
        width: constants.goal.width,
        position: { x: constants.goal.depth / 2, y: 0, z: 0 },
        rotation: { x: 0, y: 0, z: Math.PI / 2 },
      },
    ];
  }
}

export default ThreeGoalObject;

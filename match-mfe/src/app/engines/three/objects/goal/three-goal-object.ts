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
    for (let i = 0; i < params.length; i++) {
      const wall = this.buildMesh(params[i].width);
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

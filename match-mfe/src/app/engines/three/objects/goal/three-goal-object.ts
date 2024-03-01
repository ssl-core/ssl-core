import { BoxGeometry, Mesh, MeshPhongMaterial } from "three";

import ThreeBaseObject from "../three-base-object";
import constants from "../../../../../config/constants";

class ThreeGoalObject extends ThreeBaseObject {
  public update() {}

  protected buildMeshes() {
    const wallMaterial = new MeshPhongMaterial({ color: constants.goal.color });

    const wall1geometry = new BoxGeometry(
      constants.goal.depth,
      constants.goal.thickness,
      constants.goal.height
    );
    const wall1 = new Mesh(wall1geometry, wallMaterial);
    wall1.position.set(0, -constants.goal.width / 2, 0);

    const wall2geometry = new BoxGeometry(
      constants.goal.depth,
      constants.goal.thickness,
      constants.goal.height
    );
    const wall2 = new Mesh(wall2geometry, wallMaterial);
    wall2.position.set(0, constants.goal.width / 2, 0);

    const wall3geometry = new BoxGeometry(
      constants.goal.thickness,
      constants.goal.width + constants.goal.thickness,
      constants.goal.height
    );
    const wall3 = new Mesh(wall3geometry, wallMaterial);
    wall3.position.set(constants.goal.depth / 2, 0, 0);

    return [wall1, wall2, wall3];
  }
}

export default ThreeGoalObject;

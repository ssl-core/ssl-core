import { BoxGeometry, Mesh, MeshPhongMaterial } from "three";

import ThreeBaseObject from "../three-base-object";
import constants from "../../../../../config/constants";
import { fuzzyNotEqual } from "../../../../../utils/fuzzy";

class ThreeGoalObject extends ThreeBaseObject {
  private params: Field | null;
  private goalPosts: Mesh[];

  constructor() {
    super();
    this.params = null;
    this.goalPosts = [];

    this.addMeshes();
  }

  public update() {}

  public setParams(params: Field, side: number) {
    this.position.set(
      (side * (params.length + params.goal_depth)) / 2,
      0,
      constants.field.goal.height / 2
    );

    if (side < 0) {
      this.rotation.set(0, Math.PI, 0);
    }

    if (this.needsToRedraw(params)) {
      const goalPostParams = this.getGoalPostsParams(params);

      for (const [index, goalPostParam] of goalPostParams.entries()) {
        const goalPost = this.goalPosts[index];
        goalPost.geometry = new BoxGeometry(
          goalPostParam.width,
          constants.field.wall.thickness,
          constants.field.wall.height
        );
        goalPost.position.set(
          goalPostParam.position.x,
          goalPostParam.position.y,
          goalPostParam.position.z
        );
        goalPost.rotation.set(
          goalPostParam.rotation.x,
          goalPostParam.rotation.y,
          goalPostParam.rotation.z
        );
      }
    }

    this.params = params;
  }

  protected addMeshes() {
    const goalPostParams = this.getGoalPostsParams(this.params);

    for (const goalPostParam of goalPostParams) {
      const goalPost = this.buildMesh(goalPostParam.width);
      goalPost.position.set(
        goalPostParam.position.x,
        goalPostParam.position.y,
        goalPostParam.position.z
      );
      goalPost.rotation.set(
        goalPostParam.rotation.x,
        goalPostParam.rotation.y,
        goalPostParam.rotation.z
      );

      this.goalPosts.push(goalPost);
      this.add(goalPost);
    }
  }

  private needsToRedraw(params: Field) {
    return (
      !this.params ||
      fuzzyNotEqual(params.goal_depth, this.params.goal_depth) ||
      fuzzyNotEqual(params.goal_width, this.params.goal_width)
    );
  }

  private buildMesh(width: number) {
    const geometry = new BoxGeometry(
      width,
      constants.field.wall.thickness,
      constants.field.wall.height
    );
    const material = new MeshPhongMaterial({
      color: constants.field.goal.color,
      fog: false,
    });

    return new Mesh(geometry, material);
  }

  private getGoalPostsParams(params: Field | null) {
    const depth = params ? params.goal_depth : 0;
    const width = params ? params.goal_width : 0;

    return [
      {
        width: depth,
        position: { x: 0, y: -width / 2, z: 0 },
        rotation: { x: 0, y: 0, z: 0 },
      },
      {
        width: depth,
        position: { x: 0, y: width / 2, z: 0 },
        rotation: { x: 0, y: 0, z: 0 },
      },
      {
        width: width,
        position: { x: depth / 2, y: 0, z: 0 },
        rotation: { x: 0, y: 0, z: Math.PI / 2 },
      },
    ];
  }
}

export default ThreeGoalObject;

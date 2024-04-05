import ThreeBaseObject from "../three-base-object";
import ThreeWallMesh from "../../meshes/walls/three-wall-mesh";
import { fuzzyNotEqual } from "../../../../../utils/fuzzy";

class ThreeWallsObject extends ThreeBaseObject {
  private params: Field | null;
  private walls: ThreeWallMesh[];

  constructor(params?: Field) {
    super();
    this.params = params || null;
    this.walls = [];

    this.addMeshes();
  }

  public update() {}

  public setParams(params: Field) {
    if (this.needsToRedraw(params)) {
      const wallsParams = this.getWallsParams(params);

      for (const [index, goalParam] of wallsParams.entries()) {
        const wall = this.walls[index];
        wall.setWidth(goalParam.width);
        wall.position.set(
          goalParam.position.x,
          goalParam.position.y,
          goalParam.position.z
        );
        wall.rotation.set(
          goalParam.rotation.x,
          goalParam.rotation.y,
          goalParam.rotation.z
        );
      }
    }

    this.params = params;
  }

  protected addMeshes() {
    const wallsParams = this.getWallsParams(this.params);

    for (const wallParam of wallsParams) {
      const wall = new ThreeWallMesh(wallParam.width);
      wall.position.set(
        wallParam.position.x,
        wallParam.position.y,
        wallParam.position.z
      );
      wall.rotation.set(
        wallParam.rotation.x,
        wallParam.rotation.y,
        wallParam.rotation.z
      );

      this.walls.push(wall);
      this.add(wall);
    }
  }

  private needsToRedraw(params: Field) {
    return (
      !this.params ||
      fuzzyNotEqual(params.length, this.params.length) ||
      fuzzyNotEqual(params.width, this.params.width) ||
      fuzzyNotEqual(params.boundary_width, this.params.boundary_width)
    );
  }

  private getWallsParams(params: Field | null) {
    const length = params ? params.length + params.boundary_width * 2 : 0;
    const width = params ? params.width + params.boundary_width * 2 : 0;

    return [
      {
        width: length,
        position: { x: 0, y: -width / 2, z: 0 },
        rotation: { x: 0, y: 0, z: 0 },
      },
      {
        width: length,
        position: { x: 0, y: width / 2, z: 0 },
        rotation: { x: 0, y: 0, z: 0 },
      },
      {
        width: width,
        position: { x: length / 2, y: 0, z: 0 },
        rotation: { x: 0, y: 0, z: Math.PI / 2 },
      },
      {
        width: width,
        position: { x: -length / 2, y: 0, z: 0 },
        rotation: { x: 0, y: 0, z: Math.PI / 2 },
      },
    ];
  }
}

export default ThreeWallsObject;

import ThreeBaseObject from "../three-base-object";
import ThreeFieldSurfaceMesh from "../../meshes/field/three-field-surface-mesh";
import ThreeFieldMesh from "../../meshes/field/three-field-mesh";
import ThreeFieldLinesMesh from "../../meshes/field/three-field-lines-mesh";
import ThreeGoalObject from "../goal/three-goal-object";
import ThreeWallsObject from "../walls/three-walls-object";
import { fuzzyNotEqual } from "../../../../../utils/fuzzy";

class ThreeFieldObject extends ThreeBaseObject {
  private params: Field | null;
  private linesMesh: ThreeFieldLinesMesh;
  private fieldMesh: ThreeFieldMesh;
  private surfaceMesh: ThreeFieldSurfaceMesh;
  private yellowGoal: ThreeGoalObject;
  private blueGoal: ThreeGoalObject;
  private walls: ThreeWallsObject;

  constructor(params?: Field) {
    super();
    this.params = params || null;
    this.linesMesh = new ThreeFieldLinesMesh();
    this.fieldMesh = new ThreeFieldMesh();
    this.surfaceMesh = new ThreeFieldSurfaceMesh();
    this.yellowGoal = new ThreeGoalObject();
    this.blueGoal = new ThreeGoalObject();
    this.walls = new ThreeWallsObject();

    this.addMeshes();
    this.addObjects();

    if (params) {
      this.setParams(params);
    }
  }

  public update() {}

  public setParams(params: Field) {
    this.yellowGoal.setParams(params, 1);
    this.blueGoal.setParams(params, -1);
    this.walls.setParams(params);

    if (this.needsToRedraw(params)) {
      this.linesMesh.setParams(
        params.length,
        params.width,
        params.penalty_area_depth,
        params.penalty_area_width
      );
      this.fieldMesh.setParams(
        params.length,
        params.width,
        params.boundary_width
      );
      this.surfaceMesh.setParams(
        params.length,
        params.width,
        params.boundary_width
      );
    }

    this.params = params;
  }

  protected addMeshes() {
    this.add(this.linesMesh, this.fieldMesh, this.surfaceMesh);
  }

  private addObjects() {
    this.add(this.yellowGoal, this.blueGoal, this.walls);
  }

  private needsToRedraw(params: Field) {
    return (
      !this.params ||
      fuzzyNotEqual(this.params.length, params.length) ||
      fuzzyNotEqual(this.params.width, params.width) ||
      fuzzyNotEqual(this.params.boundary_width, params.boundary_width) ||
      fuzzyNotEqual(
        this.params.penalty_area_depth,
        params.penalty_area_depth
      ) ||
      fuzzyNotEqual(this.params.penalty_area_width, params.penalty_area_width)
    );
  }
}

export default ThreeFieldObject;

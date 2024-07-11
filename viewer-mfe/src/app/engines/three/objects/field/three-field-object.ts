import ThreeBaseObject from "../three-base-object";
import ThreeFieldMesh from "../../meshes/field/three-field-mesh";
import ThreeFieldLinesMesh from "../../meshes/field/three-field-lines-mesh";
import ThreeGoalObject from "../goal/three-goal-object";
import { fuzzyNotEqual } from "../../../../../utils/fuzzy";

class ThreeFieldObject extends ThreeBaseObject {
  private params: Field | null;
  private linesMesh: ThreeFieldLinesMesh;
  private fieldMesh: ThreeFieldMesh;
  private yellowGoal: ThreeGoalObject;
  private blueGoal: ThreeGoalObject;

  constructor(params?: Field) {
    super();
    this.selectable = true;
    this.params = params || null;
    this.linesMesh = new ThreeFieldLinesMesh();
    this.fieldMesh = new ThreeFieldMesh();
    this.yellowGoal = new ThreeGoalObject();
    this.blueGoal = new ThreeGoalObject();

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

    if (this.needsToRedraw(params)) {
      this.linesMesh.setParams(
        params.length,
        params.width,
        params.penaltyAreaDepth,
        params.penaltyAreaWidth
      );
      this.fieldMesh.setParams(
        params.length,
        params.width,
        params.boundaryWidth
      );
    }

    this.params = params;
  }

  protected addMeshes() {
    this.add(this.linesMesh, this.fieldMesh);
  }

  private addObjects() {
    this.add(this.yellowGoal, this.blueGoal);
  }

  private needsToRedraw(params: Field) {
    return (
      !this.params ||
      fuzzyNotEqual(this.params.length, params.length) ||
      fuzzyNotEqual(this.params.width, params.width) ||
      fuzzyNotEqual(this.params.boundaryWidth, params.boundaryWidth) ||
      fuzzyNotEqual(this.params.penaltyAreaDepth, params.penaltyAreaDepth) ||
      fuzzyNotEqual(this.params.penaltyAreaWidth, params.penaltyAreaWidth)
    );
  }
}

export default ThreeFieldObject;

import { FieldResponse } from "../types/requests";

class Field {
  private length: number;
  private width: number;
  private goalDepth: number;
  private goalWidth: number;
  private penaltyAreaDepth: number;
  private penaltyAreaWidth: number;
  private boundaryWidth: number;
  private goalCenterToPenaltyMark: number;

  constructor(data: FieldResponse) {
    this.length = data.length;
    this.width = data.width;
    this.goalDepth = data.goal_depth;
    this.goalWidth = data.goal_width;
    this.penaltyAreaDepth = data.penalty_area_depth;
    this.penaltyAreaWidth = data.penalty_area_width;
    this.boundaryWidth = data.boundary_width;
    this.goalCenterToPenaltyMark = data.goal_center_to_penalty_mark;
  }
}

export default Field;

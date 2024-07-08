export type BallResponse = {
  confidence: number;
  position: Vector3;
  velocity: Vector3;
};

export type FieldResponse = {
  length: number;
  width: number;
  goal_depth: number;
  goal_width: number;
  penalty_area_depth: number;
  penalty_area_width: number;
  boundary_width: number;
  goal_center_to_penalty_mark: number;
};

export type RobotResponse = {
  confidence: number;
  robot_id: number;
  robot_color: RobotColor;
  position: Vector2;
  angle: number;
  velocity: Vector2;
  angular_velocity: number;
  radius: number;
  height: number;
  dribbler_width: number;
};

export type FrameResponse = {
  start_time: number;
  current_time: number;
  serial_id: number;
  fps: number;
  balls: BallResponse[];
  robots: RobotResponse[];
  field: FieldResponse;
};

export type ChunkResponse = {
  end_time: number;
  frames: FrameResponse[];
};

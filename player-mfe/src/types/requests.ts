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

export type TeamResponse = {
  name: string;
  score: number;
  is_positive_half: boolean;
  robot_color: RobotColor;
  goalkeeper_id: number;
  yellow_cards: number;
  time_to_expire_active_yellow_cards: number[];
  red_cards: number;
  timeouts_left: number;
  total_timeout_time_left: number;
  fouls_committed: number;
  consecutive_ball_placement_failures: number;
  is_ball_placement_enabled: boolean;
  has_ball_placement_failures_reached_maximum: boolean;
  maximum_allowed_robots: number;
  is_robot_substitution_requested: boolean;
  is_robot_substitution_allowed: boolean;
  robot_substitutions_left: number;
  robot_substitution_time_left: number;
};

export type FrameResponse = {
  start_time: string;
  current_time: string;
  serial_id: number;
  fps: number;
  balls: BallResponse[];
  robots: RobotResponse[];
  field: FieldResponse;
  away_team: TeamResponse;
  home_team: TeamResponse;
  command: any;
  command_time: string;
  game_events: any;
  game_events_proposals: any;
  game_stage: string;
  game_stage_time_left: number;
  match_type: string;
  next_command: any;
  total_commands: number;
};

export type ChunkResponse = {
  request_time: string;
  end_time: string;
  frames: FrameResponse[];
};

export type DurationResponse = {
  start_time: number;
  end_time: number;
};

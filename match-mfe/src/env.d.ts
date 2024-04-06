/// <reference types="vite/client" />

type WebSocketMessage = {
  type: string;
  payload: any;
};

type ProxyEvent = {
  id: number;
  data: any;
};

type Vector3 = [number, number, number];

type RobotColor = "yellow" | "blue";

type Frame = {
  serial_id: number;
  effective_serial_id: number;
  fps: number;
  balls: Ball[];
  robots: Robot[];
  field: Field;
  created_at: string;
};

type Ball = {
  confidence: number;
  position: Vector3;
  velocity: Vector3;
};

type Robot = {
  confidence: number;
  robot_id: number;
  robot_color: RobotColor;
  position: Vector3;
  angle: number;
  velocity: Vector3;
  angular_velocity: number;
  radius: number;
  height: number;
  dribbler_width: number;
};

type Field = {
  serial_id: number;
  length: number;
  width: number;
  goal_depth: number;
  goal_width: number;
  penalty_area_depth: number;
  penalty_area_width: number;
  boundary_width: number;
  goal_center_to_penalty_mark: number;
};

/// <reference types="vite/client" />

type Vector2 = [number, number];
type Vector3 = [number, number, number];
type RobotColor = "yellow" | "blue";

type EventBusMessage<T = any> = {
  type: string;
  payload: T;
};

type ProxyEvent = {
  id: number;
  data: any;
};

type ThreeSyncMessage = {
  camera: (string | number | undefined)[];
  time: number;
};

type Frame = {
  serialId: number;
  startTime: Date;
  currentTime: Date;
  endTime: Date;
  fps: number;
  balls: Ball[];
  robots: Robot[];
  field: Field;
};

type Ball = {
  confidence: number;
  position: Vector3;
  velocity: Vector3;
};

type Robot = {
  confidence: number;
  robotId: number;
  robotColor: RobotColor;
  position: Vector2;
  angle: number;
  velocity: Vector2;
  angularVelocity: number;
  radius: number;
  height: number;
  dribblerWidth: number;
};

type Field = {
  length: number;
  width: number;
  goalDepth: number;
  goalWidth: number;
  penaltyAreaDepth: number;
  penaltyAreaWidth: number;
  boundaryWidth: number;
  goalCenterToPenaltyMark: number;
};

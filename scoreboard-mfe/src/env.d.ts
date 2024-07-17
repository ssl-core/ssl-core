/// <reference types="vite/client" />

type Vector2 = [number, number];
type Vector3 = [number, number, number];
type RobotColor = "yellow" | "blue";

type EventBusMessage<T = any> = {
  type: string;
  payload: T;
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
  homeTeam: Team;
  awayTeam: Team;
  command: any;
  commandTime: Date;
  gameEvents: any;
  gameEventsProposals: any;
  gameStage: string;
  gameStageTimeLeft: Date;
  matchType: string;
  nextCommand: any;
  totalCommands: number;
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

type Team = {
  name: string;
  score: number;
  isPositiveHalf: boolean;
  color: RobotColor;
  goalkeeperId: number;
  yellowCards: number;
  timeToExpireActiveYellowCards: number[];
  redCards: number;
  timeoutsLeft: number;
  totalTimeoutTimeLeft: Date;
  foulsCommitted: number;
  consecutiveBallPlacementFailures: number;
  isBallPlacementEnabled: boolean;
  hasBallPlacementFailuresReachedMaximum: boolean;
  maximumAllowedRobots: number;
  isRobotSubstitutionRequested: boolean;
  isRobotSubstitutionAllowed: boolean;
  robotSubstitutionsLeft: number;
  robotSubstitutionTimeLeft: Date;
};

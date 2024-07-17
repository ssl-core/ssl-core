import { FrameResponse } from "../types/requests";
import Ball from "./ball";
import Field from "./field";
import Robot from "./robot";
import Team from "./team";

class Frame {
  private serialId: number;
  private startTime: Date;
  private currentTime: Date;
  private endTime: Date;
  private fps: number;
  private balls: Ball[];
  private robots: Robot[];
  private field: Field;
  private homeTeam: Team;
  private awayTeam: Team;
  private command: any;
  private commandTime: Date;
  private gameEvents: any;
  private gameEventsProposals: any;
  private gameStage: string;
  private gameStageTimeLeft: Date;
  private matchType: string;
  private nextCommand: any;
  private totalCommands: number;

  constructor(data: FrameResponse, endTime?: number) {
    this.serialId = data.serial_id;
    this.startTime = new Date(data.start_time);
    this.currentTime = new Date(data.current_time);
    this.endTime = new Date(endTime ?? data.current_time);
    this.fps = data.fps;
    this.balls = data.balls.map((response) => new Ball(response));
    this.robots = data.robots.map((response) => new Robot(response));
    this.field = new Field(data.field);
    this.homeTeam = new Team(data.home_team);
    this.awayTeam = new Team(data.away_team);
    this.command = data.command;
    this.commandTime = new Date(data.command_time);
    this.gameEvents = data.game_events;
    this.gameEventsProposals = data.game_events_proposals;
    this.gameStage = data.game_stage;
    this.gameStageTimeLeft = new Date(data.game_stage_time_left);
    this.matchType = data.match_type;
    this.nextCommand = data.next_command;
    this.totalCommands = data.total_commands;
  }

  public getCurrentTimestamp() {
    return this.currentTime.getTime();
  }

  public parseTimeMetadata() {
    const currentTime = Math.round(
      (this.currentTime.getTime() - this.startTime.getTime()) / 1000
    );
    const duration = Math.round(
      (this.endTime.getTime() - this.startTime.getTime()) / 1000
    );

    return {
      currentTime,
      duration,
    };
  }

  public secondsToTimestamp(seconds: number) {
    return this.startTime.getTime() + seconds * 1000;
  }
}

export default Frame;

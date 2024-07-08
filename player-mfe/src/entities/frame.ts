import { FrameResponse } from "../types/requests";
import Ball from "./ball";
import Field from "./field";
import Robot from "./robot";

class Frame {
  private serialId: number;
  private startTime: Date;
  private currentTime: Date;
  private endTime: Date;
  private fps: number;
  private balls: Ball[];
  private robots: Robot[];
  private field: Field;

  constructor(data: FrameResponse, endTime?: number) {
    this.serialId = data.serial_id;
    this.startTime = new Date(data.start_time);
    this.currentTime = new Date(data.current_time);
    this.endTime = new Date(endTime ?? data.current_time);
    this.fps = data.fps;
    this.balls = data.balls.map((response) => new Ball(response));
    this.robots = data.robots.map((response) => new Robot(response));
    this.field = new Field(data.field);
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

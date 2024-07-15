import { ChunkResponse } from "../types/requests";
import Frame from "./frame";

class Chunk {
  private frames: Frame[];
  private lastFrame: Frame | null;
  private requestTime: Date;
  private endTime: Date;

  constructor(data: ChunkResponse) {
    this.frames = data.frames.map(
      (frame) => new Frame(frame, Date.parse(data.end_time))
    );
    this.lastFrame = null;
    this.requestTime = new Date(data.request_time);
    this.endTime = new Date(data.end_time);
  }

  public nextFrame() {
    this.lastFrame = this.frames.shift() ?? null;
    return this.lastFrame;
  }

  public getLastFrame() {
    return this.lastFrame;
  }

  public getRequestTime() {
    return this.requestTime;
  }

  public getEndTime() {
    return this.endTime;
  }

  public getLastTimestamp() {
    return this.frames[this.frames.length - 1]?.getCurrentTimestamp();
  }
}

export default Chunk;

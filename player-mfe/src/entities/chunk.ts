import { ChunkResponse } from "../types/requests";
import Frame from "./frame";

class Chunk {
  private frames: Frame[];
  private endTime: Date;

  constructor(data: ChunkResponse) {
    this.frames = data.frames.map((frame) => new Frame(frame, data.end_time));
    this.endTime = new Date(data.end_time);
  }

  public nextFrame() {
    return this.frames.shift();
  }

  public getLastTimestamp() {
    return this.frames[this.frames.length - 1].getCurrentTimestamp();
  }
}

export default Chunk;

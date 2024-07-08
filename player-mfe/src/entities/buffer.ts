import Chunk from "./chunk";
import Frame from "./frame";

class Buffer {
  private chunks: Chunk[];
  private interval: number | null;

  constructor() {
    this.chunks = [];
    this.interval = null;
  }

  public add(chunk: Chunk) {
    this.chunks.push(chunk);
  }

  public clear() {
    this.chunks = [];
  }

  public isEmpty() {
    return this.chunks.length === 0;
  }

  public play(options: {
    timestamp?: number;
    onFrame: (frame: Frame) => void;
    onFetch: (timestamp: number) => void;
  }) {
    if (this.interval) {
      clearInterval(this.interval);
      this.interval = null;
    }

    // TODO: optimize if timestamp is in range of current chunks
    if (this.isEmpty() || options.timestamp) {
      this.clear();
      options.onFetch(options.timestamp ?? 0);
    }

    // TODO: Move to worker thread?
    // TODO: Wait the time between two frames (get timestamp of the first frame and then wait until the timestamp of the next frame)
    this.interval = setInterval(() => {
      if (this.isEmpty()) {
        return;
      }

      const frame = this.chunks[0].nextFrame();

      if (!frame) {
        this.chunks.shift();
        options.onFetch(this.chunks[0].getLastTimestamp());
        return;
      }

      options.onFrame(frame);
    }, 1000 / 60);
  }
}

export default Buffer;

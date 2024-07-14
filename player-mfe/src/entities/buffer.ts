import Chunk from "./chunk";
import Frame from "./frame";

class Buffer {
  private chunks: Chunk[];
  private interval: number | null;
  private fetchingChunks: number[];

  constructor() {
    this.chunks = [];
    this.interval = null;
    this.fetchingChunks = [];
  }

  public add(chunk: Chunk) {
    const requestTime = chunk.getRequestTime().getTime();

    if (!this.fetchingChunks.some((timestamp) => timestamp === requestTime)) {
      return;
    }

    this.chunks.push(chunk);
    this.fetchingChunks = this.fetchingChunks.filter(
      (timestamp) => timestamp !== requestTime
    );
  }

  public clear() {
    this.chunks = [];
  }

  public isEmpty() {
    return this.chunks.length === 0;
  }

  public reset() {
    this.fetchingChunks = [];

    if (!this.interval) {
      return;
    }

    clearInterval(this.interval);
    this.interval = null;
  }

  public isFetching() {
    return this.fetchingChunks.length > 0;
  }

  public isLoading() {
    return this.isFetching() && this.isEmpty();
  }

  public play(options: {
    timestamp?: number;
    onFrame: (frame: Frame) => void;
    onFetch: (timestamp: number) => void;
  }) {
    this.reset();

    const { timestamp, onFrame, onFetch } = options;

    // TODO: optimize if timestamp is in range of current chunks
    if (this.isEmpty() || timestamp) {
      this.clear();
      this.fetch(timestamp ?? 0, onFetch);
    }

    let waitTicks = 0;

    // TODO: Move to worker thread
    this.interval = setInterval(() => {
      if (this.isEmpty()) {
        return;
      }

      if (waitTicks > 0) {
        waitTicks--;
        return;
      }

      if (this.chunks.length < 2 && !this.isFetching()) {
        this.fetch(this.chunks[0].getLastTimestamp(), onFetch);
      }

      const lastFrame = this.chunks[0].getLastFrame();
      const frame = this.chunks[0].nextFrame();

      if (!frame) {
        this.chunks.shift();
        return;
      }

      if (lastFrame) {
        waitTicks = Math.round(
          (frame.getCurrentTimestamp() - lastFrame.getCurrentTimestamp()) / 1000
        );
      }

      onFrame(frame);
    }, 1);
  }

  private fetch(timestamp: number, fn: (timestamp: number) => void) {
    this.fetchingChunks.push(timestamp);
    fn(timestamp);
  }
}

export default Buffer;

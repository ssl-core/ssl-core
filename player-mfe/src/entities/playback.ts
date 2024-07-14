import SocketHandler from "../lib/socket/socket-handler";
import Buffer from "./buffer";
import Chunk from "./chunk";
import Frame from "./frame";
import { PlaybackUpdateEvent } from "../events/playback-update";
import {
  ChunkResponse,
  DurationResponse,
  FrameResponse,
} from "../types/requests";

export enum PlaybackState {
  Live = "live",
  Play = "play",
  Pause = "pause",
  Stop = "connect",
  Disconnected = "disconnect",
}

class Playback {
  private socket: SocketHandler;
  private state: PlaybackState;
  private listeners: Record<string, Function[]>;
  private currentFrame: Frame | null;
  private currentTime: number;
  private duration: number;
  private buffer: Buffer;
  private eventBus: BroadcastChannel;

  constructor(socket: SocketHandler) {
    this.socket = socket;
    this.state = PlaybackState.Disconnected;
    this.listeners = {};
    this.currentFrame = null;
    this.currentTime = 0;
    this.duration = 0;
    this.buffer = new Buffer();
    this.eventBus = new BroadcastChannel("event-bus");

    this.socket.addEventListener("state", (state: PlaybackState) =>
      this.changeState(state)
    );
    this.socket.addEventListener("frame", (frame: FrameResponse) =>
      this.receiveFrame(frame)
    );
    this.socket.addEventListener("chunk", (chunk: ChunkResponse) =>
      this.receiveChunk(chunk)
    );
    this.socket.addEventListener("duration", (duration: DurationResponse) =>
      this.receiveDuration(duration)
    );
  }

  public live() {
    if (this.state === PlaybackState.Disconnected) {
      throw new Error("Disconnected");
    }

    if ([PlaybackState.Live].includes(this.state)) {
      return;
    }

    this.buffer.reset();
    this.socket.playLiveStream();
  }

  public seek(seconds: number) {
    if (!this.currentFrame) {
      this.live();
      return;
    }

    this.pause();
    this.play(this.currentFrame.secondsToTimestamp(seconds));
  }

  public play(timestamp?: number) {
    if (this.state === PlaybackState.Disconnected) {
      throw new Error("Disconnected");
    }

    if (this.state === PlaybackState.Stop && !timestamp) {
      this.live();
      return;
    }

    this.buffer.play({
      timestamp: timestamp,
      onFrame: (frame: Frame) => {
        this.currentFrame = frame;
        this.update();
      },
      onFetch: (timestamp: number) => {
        this.socket.fetchReplay(timestamp);
      },
    });
  }

  public pause() {
    if (this.state === PlaybackState.Disconnected) {
      throw new Error("Disconnected");
    }

    if ([PlaybackState.Stop, PlaybackState.Pause].includes(this.state)) {
      return;
    }

    this.buffer.reset();
    this.socket.pauseLiveStream();
  }

  public changeState(state: PlaybackState) {
    this.state = state;
    this.update();
  }

  public receiveFrame(payload: FrameResponse) {
    const frame = new Frame(payload);
    this.currentFrame = frame;
    this.update();
  }

  public receiveChunk(payload: ChunkResponse) {
    console.log("Received chunk", payload);

    const chunk = new Chunk(payload);
    this.buffer.add(chunk);
  }

  public receiveDuration(payload: DurationResponse) {
    const newDuration = Math.round(
      (payload.end_time - payload.start_time) / 1000
    );

    this.duration = Math.max(newDuration, this.duration);

    const data: PlaybackUpdateEvent = {
      currentTime: this.currentTime,
      duration: this.duration,
      isPlaying:
        this.state === PlaybackState.Play || this.state === PlaybackState.Live,
      isLive: this.state === PlaybackState.Live,
      isLoading: this.buffer.isLoading(),
      frame: this.currentFrame,
    };

    this.dispatchEvent("update", data);
  }

  public addEventListener(event: string, listener: Function) {
    if (!this.listeners[event]) {
      this.listeners[event] = [];
    }

    this.listeners[event].push(listener);
  }

  public removeEventListener(event: string, listener: Function) {
    if (!this.listeners[event]) {
      return;
    }

    this.listeners[event] = this.listeners[event].filter((l) => l !== listener);
  }

  private update() {
    if (!this.currentFrame) {
      return;
    }

    const { currentTime, duration } = this.currentFrame.parseTimeMetadata();

    this.currentTime = currentTime;
    this.duration = Math.max(duration, this.duration);

    const data: PlaybackUpdateEvent = {
      currentTime: this.currentTime,
      duration: this.duration,
      isPlaying:
        this.state === PlaybackState.Play || this.state === PlaybackState.Live,
      isLive: this.state === PlaybackState.Live,
      isLoading: this.buffer.isLoading(),
      frame: this.currentFrame,
    };

    this.eventBus.postMessage({ type: "frame", payload: this.currentFrame });

    this.dispatchEvent("update", data);
  }

  private dispatchEvent(event: string, payload: any) {
    if (!this.listeners[event]) {
      return;
    }

    this.listeners[event].forEach((listener) => listener(payload));
  }
}

export default Playback;

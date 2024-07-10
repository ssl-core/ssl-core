import SocketHandler from "../lib/socket/socket-handler";
import Buffer from "./buffer";
import Chunk from "./chunk";
import Frame from "./frame";
import { PlaybackUpdateEvent } from "../events/playback-update";
import { ChunkResponse, FrameResponse } from "../types/requests";

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
  private buffer: Buffer;
  private eventBus: BroadcastChannel;

  constructor(socket: SocketHandler) {
    this.socket = socket;
    this.state = PlaybackState.Disconnected;
    this.listeners = {};
    this.currentFrame = null;
    this.buffer = new Buffer();

    this.socket.addEventListener("state", (state: PlaybackState) =>
      this.changeState(state)
    );
    this.socket.addEventListener("frame", (frame: FrameResponse) =>
      this.receiveFrame(frame)
    );
    this.socket.addEventListener("chunk", (chunk: ChunkResponse) =>
      this.receiveChunk(chunk)
    );
    this.eventBus = new BroadcastChannel("event-bus");
  }

  public live() {
    if (this.state === PlaybackState.Disconnected) {
      throw new Error("Disconnected");
    }

    if ([PlaybackState.Live].includes(this.state)) {
      return;
    }

    this.socket.playLiveStream();
  }

  public seek(seconds: number) {
    if (!this.currentFrame) {
      return;
    }

    this.pause();
    this.play(this.currentFrame.secondsToTimestamp(seconds));
  }

  public play(timestamp?: number) {
    if (this.state === PlaybackState.Disconnected) {
      throw new Error("Disconnected");
    }

    console.log(this.state, timestamp);

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

    this.socket.pauseLiveStream();
  }

  public changeState(state: PlaybackState) {
    console.log(state);
    this.state = state;
    this.update();
  }

  public receiveFrame(payload: FrameResponse) {
    console.log("receiveFrame", payload);
    const frame = new Frame(payload);
    this.currentFrame = frame;
    this.update();
  }

  public receiveChunk(payload: ChunkResponse) {
    const chunk = new Chunk(payload);
    this.buffer.add(chunk);
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

    const data: PlaybackUpdateEvent = {
      currentTime: currentTime,
      duration: duration,
      isPlaying:
        this.state === PlaybackState.Play || this.state === PlaybackState.Live,
      frame: this.currentFrame,
    };

    this.eventBus.postMessage({ type: "frame", payload: this.currentFrame });
    console.log({ type: "frame", payload: this.currentFrame });

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

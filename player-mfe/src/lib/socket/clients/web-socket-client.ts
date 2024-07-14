import SocketClient from "./socket-client";
import { ChunkResponse, FrameResponse } from "../../../types/requests";

class WebSocketClient implements SocketClient {
  private socket: WebSocket | null;
  private playing: boolean;
  private connected: boolean;

  constructor() {
    this.socket = null;
    this.playing = false;
    this.connected = false;
  }

  public connect(address: string) {
    if (this.connected) {
      return;
    }

    this.socket = new WebSocket(address);
    this.socket.addEventListener("open", () => this.receiveStream());
    this.socket.addEventListener("message", (event) =>
      this.handleMessage(event)
    );
    this.sendState("connect");
    this.connected = true;
  }

  public disconnect() {
    if (!this.socket) {
      throw new Error("Socket not initialized");
    }

    if (!this.connected) {
      return;
    }

    this.socket.removeEventListener("message", this.handleMessage);
    this.socket.removeEventListener("open", this.receiveStream);
    this.socket.close();

    this.sendState("disconnect");
    this.connected = false;
  }

  public isConnected() {
    return this.connected;
  }

  public live() {
    if (!this.socket) {
      throw new Error("Socket not initialized");
    }

    this.sendState("live");
    this.playing = true;
  }

  public play(timestamp: number) {
    if (!this.socket) {
      throw new Error("Socket not initialized");
    }

    this.sendState("play");
    this.fetchReplay(timestamp);
    this.playing = true;
  }

  public pause() {
    if (!this.socket) {
      throw new Error("Socket not initialized");
    }

    this.sendState("pause");
    this.playing = false;
  }

  public isPlaying() {
    return this.playing;
  }

  public send(message: any) {
    if (!this.socket) {
      throw new Error("Socket not initialized");
    }

    this.socket.send(JSON.stringify(message));
  }

  private receiveStream() {
    if (!this.socket) {
      throw new Error("Socket not initialized");
    }

    this.send({ event: "receive-live-stream" });
  }

  private fetchReplay(timestamp: number): void {
    if (!this.socket) {
      throw new Error("Socket not initialized");
    }

    this.send({ event: "get-replay-chunk", data: { timestamp } });
  }

  private handleMessage(event: MessageEvent<string>) {
    const { type, payload } = JSON.parse(event.data);

    switch (type) {
      case "frame":
        this.handleFrame(payload);
        break;
      case "chunk":
        this.handleChunk(payload);
        break;
      default:
        break;
    }
  }

  private handleFrame(frame: FrameResponse) {
    if (!this.playing) {
      self.postMessage({
        type: "duration",
        payload: {
          start_time: frame.start_time,
          end_time: frame.current_time,
        },
      });
      return;
    }

    self.postMessage({ type: "frame", payload: frame });
  }

  private handleChunk(chunk: ChunkResponse) {
    if (!this.playing) {
      self.postMessage({
        type: "duration",
        payload: {
          start_time: chunk.frames[0].start_time,
          end_time: chunk.end_time,
        },
      });
      return;
    }

    self.postMessage({ type: "chunk", payload: chunk });
  }

  private sendState(state: string) {
    self.postMessage({ type: "state", payload: state });
  }
}

export default WebSocketClient;

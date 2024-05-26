import SocketClient from "./socket-client";
import { Frame } from "../../../entities/frame";

class WebSocketClient implements SocketClient {
  private socket: WebSocket | null;

  constructor() {
    this.socket = null;
  }

  public connect(address: string) {
    this.socket = new WebSocket(address);
    this.socket.addEventListener("message", this.handleMessage);
    this.receiveStream();
  }

  public disconnect() {
    if (!this.socket) {
      throw new Error("Socket not initialized");
    }

    this.socket.removeEventListener("message", this.handleMessage);
    this.socket.close();
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

    this.socket.addEventListener("open", () => {
      const message = JSON.stringify({ event: "receive-live-stream" });
      this.socket?.send(message);
    });
  }

  private handleMessage(event: MessageEvent<string>) {
    const { type, payload } = JSON.parse(event.data);

    switch (type) {
      case "frame":
        this.handleFrame(payload);
        break;
      default:
        break;
    }
  }

  private handleFrame(frame: Frame) {
    self.postMessage({ type: "frame", payload: frame });
  }
}

export default WebSocketClient;

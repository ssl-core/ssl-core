import Channels from "../../../config/channels";
import BaseSocketManager from "../base-socket-manager";

class WebSocketManager extends BaseSocketManager {
  private socket: WebSocket | null;
  private engineChannel: BroadcastChannel;

  constructor() {
    super();
    this.socket = null;
    this.engineChannel = new BroadcastChannel(Channels.Engine);
  }

  public connect(address: string) {
    this.socket = new WebSocket(address);
    this.addMessageListener();
    this.receiveLiveStream();
  }

  public disconnect() {
    if (!this.socket) {
      throw new Error("Socket not initialized");
    }

    this.socket.close();
  }

  public send(message: any) {
    if (!this.socket) {
      throw new Error("Socket not initialized");
    }

    this.socket.send(JSON.stringify(message));
  }

  private addMessageListener() {
    if (!this.socket) {
      throw new Error("Socket not initialized");
    }

    this.socket.addEventListener("message", (event: MessageEvent<string>) => {
      const { type, payload }: WebSocketMessage = JSON.parse(event.data);

      switch (type) {
        case "frame":
          this.handleFrame(payload as Frame);
          break;
        default:
          break;
      }
    });
  }

  private receiveLiveStream() {
    if (!this.socket) {
      throw new Error("Socket not initialized");
    }

    this.socket.addEventListener("open", () => {
      const message = JSON.stringify({ event: "receive-live-stream" });
      this.socket?.send(message);
    });
  }

  private handleFrame(frame: Frame) {
    this.engineChannel.postMessage(frame);
  }
}

export default WebSocketManager;

import Channels from "../../../config/channels";

class WebSocketManager {
  private socket: WebSocket | null;
  private engineChannel: BroadcastChannel;
  private isSendingMessage: boolean;

  constructor() {
    this.socket = null;
    this.engineChannel = new BroadcastChannel(Channels.Engine);
    this.isSendingMessage = false;
  }

  public initialize(address: string) {
    this.socket = new WebSocket(address);
    this.addMessageListener();
    this.receiveLiveStream();
  }

  public send(message: any) {
    if (!this.socket) {
      throw new Error("Socket not initialized");
    }

    this.socket.send(JSON.stringify(message));
  }

  public close() {
    if (!this.socket) {
      throw new Error("Socket not initialized");
    }

    this.socket.close();
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

import Channels from "../../../config/channels";

class WebSocketManager {
  private socket: WebSocket | null;
  private engineChannel: BroadcastChannel;

  constructor() {
    this.socket = null;
    this.engineChannel = new BroadcastChannel(Channels.Engine);
  }

  public initialize(address: string) {
    this.socket = new WebSocket(address);
    this.addMessageListener();
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

  private handleFrame(frame: Frame) {
    this.engineChannel.postMessage(frame);
  }
}

export default WebSocketManager;

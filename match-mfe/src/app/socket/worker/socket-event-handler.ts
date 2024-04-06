import WebSocketManager from "../web/web-socket-manager";
import SocketEventTypes from "./socket-event-types";

class SocketEventHandler {
  private webSocketManager: WebSocketManager;

  constructor(webSocketManager: WebSocketManager) {
    this.webSocketManager = webSocketManager;
  }

  public handleEvent(type: string, payload: any) {
    switch (type) {
      case SocketEventTypes.Initialize:
        const { address } = payload;
        this.webSocketManager.initialize(address as string);
        break;
      default:
        throw new Error(`Unknown event type: ${type}`);
    }
  }
}

export default SocketEventHandler;

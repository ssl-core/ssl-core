import BaseSocketManager from "../base-socket-manager";
import SocketEventTypes from "./socket-event-types";

class SocketEventHandler {
  private socketManager: BaseSocketManager;

  constructor(socketManager: BaseSocketManager) {
    this.socketManager = socketManager;
  }

  public handleEvent(type: string, payload: any) {
    switch (type) {
      case SocketEventTypes.Connect:
        const { address } = payload;
        this.socketManager.connect(address as string);
        break;
      default:
        throw new Error(`Unknown event type: ${type}`);
    }
  }
}

export default SocketEventHandler;

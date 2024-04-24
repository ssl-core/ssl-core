import BaseSocketManager from "./base-socket-manager";
import TestSocketManager from "./test/test-socket-manager";
import WebSocketManager from "./web/web-socket-manager";

class SocketManagerFactory {
  static createManager(type: string): BaseSocketManager {
    switch (type) {
      case "websocket":
        return new WebSocketManager();
      case "test":
        return new TestSocketManager(60);
    }

    throw new Error(`Unknown socket manager type: ${type}`);
  }
}

export default SocketManagerFactory;

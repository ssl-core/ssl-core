import SocketClient from "../clients/socket-client";
import WebSocketClient from "../clients/web-socket-client";
import FakeSocketClient from "../clients/fake-socket-client";

class SocketClientFactory {
  public static create(type: string): SocketClient {
    switch (type) {
      case "websocket":
        return new WebSocketClient();
      case "fake":
        return new FakeSocketClient(60);
    }

    throw new Error(`Unknown socket client type: ${type}`);
  }
}

export default SocketClientFactory;

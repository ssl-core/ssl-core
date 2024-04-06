import WebSocketManager from "../web/web-socket-manager";
import SocketEventHandler from "./socket-event-handler";

const webSocketManager = new WebSocketManager();
const handler = new SocketEventHandler(webSocketManager);

self.onmessage = (event) => {
  const { data } = event;
  handler.handleEvent(data.type, data.payload);
};

import SocketManagerFactory from "../socket-manager-factory";
import SocketEventHandler from "./socket-event-handler";

const socketManager = SocketManagerFactory.createManager("test");
const handler = new SocketEventHandler(socketManager);

self.onmessage = (event) => {
  const { data } = event;
  handler.handleEvent(data.type, data.payload);
};

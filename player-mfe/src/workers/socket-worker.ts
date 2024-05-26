import SocketClientFactory from "../lib/socket/factories/socket-client-factory";

const socketClient = SocketClientFactory.create("fake");

self.onmessage = (event) => {
  const { type, payload } = event.data;

  switch (type) {
    case "connect":
      socketClient.connect(payload.address as string);
      break;
    default:
      throw new Error(`Unknown event type: ${type}`);
  }
};

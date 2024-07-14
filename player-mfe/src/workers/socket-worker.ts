import SocketClientFactory from "../lib/socket/factories/socket-client-factory";

const socketClient = SocketClientFactory.create("fake");

self.onmessage = (event) => {
  const { type, payload } = event.data;

  switch (type) {
    case "connect":
      socketClient.connect(payload.address as string);
      break;
    case "disconnect":
      socketClient.disconnect();
      break;
    case "play":
      socketClient.live();
      break;
    case "pause":
      socketClient.pause();
      break;
    case "fetch":
      socketClient.play(payload.timestamp as number);
      break;
    default:
      throw new Error(`Unknown event type: ${type}`);
  }
};

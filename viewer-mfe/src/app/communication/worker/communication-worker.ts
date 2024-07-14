import CommunicationManagerFactory from "../communication-manager-factory";
import CommunicationEventHandler from "./communication-event-handler";

const communicationManager =
  CommunicationManagerFactory.createManager("eventBus");
const handler = new CommunicationEventHandler(communicationManager);

self.onmessage = (event) => {
  const { data } = event;
  handler.handleEvent(data.type, data.payload);
};

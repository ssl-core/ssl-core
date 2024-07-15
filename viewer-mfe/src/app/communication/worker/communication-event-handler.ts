import BaseCommunicationManager from "../base-communication-manager";
import CommunicationEventTypes from "./communication-event-types";

class CommunicationEventHandler {
  private communicationManager: BaseCommunicationManager;

  constructor(communicationManager: BaseCommunicationManager) {
    this.communicationManager = communicationManager;
  }

  public handleEvent(type: string, payload: any) {
    switch (type) {
      case CommunicationEventTypes.Connect:
        const { address } = payload;
        this.communicationManager.connect(address as string);
        break;
      default:
        throw new Error(`Unknown event type: ${type}`);
    }
  }
}

export default CommunicationEventHandler;

import BaseCommunicationManager from "./base-communication-manager";
import TestCommunicationManager from "./test/test-communication-manager";
import EventBusCommunicationManager from "./event-bus/event-bus-communication-manager";

class CommunicationManagerFactory {
  static createManager(type: string): BaseCommunicationManager {
    switch (type) {
      case "eventBus":
        return new EventBusCommunicationManager();
      case "test":
        return new TestCommunicationManager(60);
    }

    throw new Error(`Unknown socket manager type: ${type}`);
  }
}

export default CommunicationManagerFactory;

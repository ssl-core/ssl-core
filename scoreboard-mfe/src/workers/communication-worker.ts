import CommunicationManagerFactory from "../communication/communication-manager-factory";

const manager = CommunicationManagerFactory.createManager("eventBus");
manager.connect();

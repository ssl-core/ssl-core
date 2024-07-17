abstract class BaseCommunicationManager {
  public abstract connect(): void;
  public abstract disconnect(): void;
  public abstract send(message: any): void;
}

export default BaseCommunicationManager;

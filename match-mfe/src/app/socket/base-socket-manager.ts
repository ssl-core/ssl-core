abstract class BaseSocketManager {
  public abstract connect(address: string): void;
  public abstract disconnect(): void;
  public abstract send(message: any): void;
}

export default BaseSocketManager;

interface SocketClient {
  connect(address: string): void;
  disconnect(): void;
  play(): void;
  pause(): void;
  send(message: any): void;
}

export default SocketClient;

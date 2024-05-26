interface SocketClient {
  connect(address: string): void;
  disconnect(): void;
  send(message: any): void;
}

export default SocketClient;

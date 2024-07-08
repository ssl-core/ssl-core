interface SocketClient {
  connect(address: string): void;
  disconnect(): void;
  isConnected(): boolean;
  live(): void;
  play(timestamp: number): void;
  pause(): void;
  isPlaying(): boolean;
  send(message: any): void;
}

export default SocketClient;

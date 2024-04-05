import socket


class MulticastUdpSenderSocket:
    def __init__(self):
        self.socket = socket.socket(
            socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP
        )

    def send(self, message, ip, port):
        self.sock.sendto(message, (ip, port))

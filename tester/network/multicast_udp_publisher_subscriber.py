import socket
import struct

from google.protobuf.message import Message as Protobuf


class MulticastUdpPublisherSocket:
    def __init__(self, ip, port):
        self.socket = socket.socket(
            socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP
        )
        self.ip = ip
        self.port = port

    def send(self, message):
        if isinstance(message, str):
            self.socket.sendto(message.encode(), (self.ip, self.port))
        elif isinstance(message, bytes):
            self.socket.sendto(message, (self.ip, self.port))
        elif isinstance(message, Protobuf):
            self.socket.sendto(message.SerializeToString(), (self.ip, self.port))
        else:
            raise ValueError(f"Invalid message type: {type(message)}.")


class MulticastUdpSubscriberSocket:
    def __init__(self, ip_address, port):
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

        # Bind the socket to the network interface and port
        self.socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.socket.bind(("", port))

        # Set the network interface for multicast
        membership_request = struct.pack(
            "4sL", socket.inet_aton(ip_address), socket.INADDR_ANY
        )
        self.socket.setsockopt(
            socket.IPPROTO_IP, socket.IP_ADD_MEMBERSHIP, membership_request
        )

    def receive(self, buffer_size=2048):
        return self.socket.recv(buffer_size)

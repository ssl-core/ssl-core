import socket
import netifaces

from typing import Dict
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
    @staticmethod
    def _all_network_interfaces() -> Dict[str, str]:
        result = {}
        for network_interface in netifaces.interfaces():
            interface_addresses = netifaces.ifaddresses(network_interface)
            if netifaces.AF_INET in interface_addresses:
                result[network_interface] = interface_addresses[netifaces.AF_INET][0][
                    "addr"
                ]

        return result

    def __init__(self, ip_address, port, network_interface):
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

        # Get the network interface address
        network_interfaces = MulticastUdpSubscriberSocket._all_network_interfaces()
        if network_interface not in network_interfaces:
            raise ValueError(f"Invalid network interface: {network_interface}.")

        network_interface = network_interfaces[network_interface]

        # Bind the socket to the network interface and port
        self.socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.socket.bind((ip_address, port))

        # Set the network interface for multicast
        membership = socket.inet_aton(ip_address) + socket.inet_aton(network_interface)
        self.socket.setsockopt(socket.IPPROTO_IP, socket.IP_ADD_MEMBERSHIP, membership)

    def receive(self, buffer_size=2048):
        return self.socket.recv(buffer_size)

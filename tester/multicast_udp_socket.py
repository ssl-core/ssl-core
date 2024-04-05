import socket
import struct
from typing import Dict

import netifaces


class MulticastUdpSocket:
    def __init__(self):
        # Create a UDP Socket
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)
        self.sock.setsockopt(socket.IPPROTO_IP, socket.IP_MULTICAST_TTL, 2)

    @staticmethod
    def _all_network_interfaces() -> Dict[str, str]:
        result = {}
        for network_interface in netifaces.interfaces():
            interface_addresses = netifaces.ifaddresses(network_interface)
            if netifaces.AF_INET in interface_addresses:
                result[network_interface] = interface_addresses[netifaces.AF_INET][0][
                    "addr"
                ]

        print(f"ifconfig: {result}")

        return result

    def bind(self, ip_address, port, network_interface):
        # Get the network interface address
        network_interfaces = MulticastUdpSocket._all_network_interfaces()
        if network_interface not in network_interfaces:
            raise ValueError(f"Invalid network interface: {network_interface}.")

        network_interface = network_interfaces[network_interface]

        # Bind the socket to the network interface and port
        self.socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.socket.bind((ip_address, port))

        # Set the network interface for multicast
        membership = socket.inet_aton(ip_address) + socket.inet_aton(network_interface)
        self.socket.setsockopt(socket.IPPROTO_IP, socket.IP_ADD_MEMBERSHIP, membership)

        print("bind worked")

    def receive(self, buffer_size=2048):
        return self.socket.recv(buffer_size)

    def send(self, message, multicast_group, port):
        # MCAST_GRP = '224.1.1.1'
        # MCAST_PORT = 5007
        # self.socket.setsockopt(socket.IPPROTO_IP, socket.IP_MULTICAST_TTL, 32)
        # self.socket.sendto('Hello World!', (MCAST_GRP, MCAST_PORT))

        # Set the time-to-live for the message
        # self.socket.setsockopt(socket.IPPROTO_IP, socket.IP_MULTICAST_TTL, 32)
        print(multicast_group, port)

        # Send the message
        self.sock.sendto(message, (multicast_group, port))
        print("enviouuu!")

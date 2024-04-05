import argparse
import datetime as dt

from network.multicast_udp_receiver_socket import MulticastUdpReceiverSocket
from protocols.third_party.detection.raw_wrapper_pb2 import SSL_WrapperPacket


if __name__ == "__main__":
    # Parse arguments
    parser = argparse.ArgumentParser()
    parser.add_argument("--raw_ip_address", default="224.5.23.2")
    parser.add_argument("--raw_port", default=10020, type=int)
    parser.add_argument("--network_interface", required=True)

    args = parser.parse_args()

    # Create UDP socket to receive raw vision packets
    raw_udp_socket = MulticastUdpReceiverSocket()
    raw_udp_socket.bind(args.raw_ip_address, args.raw_port, args.network_interface)

    while True:
        # Receive vision packet
        data = raw_udp_socket.receive()
        raw_packet = SSL_WrapperPacket()

        try:
            raw_packet.ParseFromString(data)
            print(f"raw_packet: {raw_packet}")
        except TypeError:
            print(f"{dt.datetime.now()}: the received raw packet was not parsed.")
            continue

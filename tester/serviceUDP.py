import argparse
import datetime as dt

from multicast_udp_socket import MulticastUdpSocket
from protocols.third_party.detection.raw_wrapper_pb2 import SSL_WrapperPacket


if __name__ == "__main__":
    # Parse arguments
    parser = argparse.ArgumentParser()
    parser.add_argument("--raw_ip_address", default="224.5.23.2")
    parser.add_argument("--raw_port", default=10006, type=int)
    parser.add_argument("--tracked_ip_address", default="224.5.23.2")
    parser.add_argument("--tracked_port", default=10010, type=int)
    parser.add_argument("--network_interface", required=True)
    parser.add_argument("--type", default="raw", choices=["raw", "tracked"])

    args = parser.parse_args()

    # Create UDP socket to receive raw vision packets
    raw_udp_socket = MulticastUdpSocket()
    raw_udp_socket.bind(args.raw_ip_address, args.raw_port, args.network_interface)

    while True:
        print(f"receiving before... {args.raw_port} {args.raw_ip_address}")

        # Receive vision packet
        data = raw_udp_socket.receive()
        raw_packet = SSL_WrapperPacket()

        print("receiving...")

        try:
            raw_packet.ParseFromString(data)
            print(raw_packet)
        except TypeError:
            print(f"{dt.datetime.now()}: the received raw packet was not parsed.")
            continue

        # TODO: Sleep to reduce CPU usage.

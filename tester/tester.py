import json
import argparse
import sys
import time

from network.multicast_udp_publisher_subscriber import (
    MulticastUdpPublisherSocket,
    MulticastUdpSubscriberSocket,
)
from network.zmq_publisher_subscriber import ZmqPublisherSocket, ZmqSubscriberSocket
from utility.protobuf import parse_from_textpb, serialize
from pathlib import Path


def create_publisher(request):
    if "zmq" in request:
        zmq_request = request["zmq"]
        return ZmqPublisherSocket(zmq_request["topic"], zmq_request["address"])

    elif "udp_multicast" in request:
        udp_request = request["udp_multicast"]
        return MulticastUdpPublisherSocket(udp_request["ip"], udp_request["port"])

    else:
        sys.exit("[error] create_publisher: no publisher found from json.")


def create_message(request):
    message_path = f'messages/{request["message_path_pb"]}'

    textpb = Path(message_path).read_text()
    return parse_from_textpb(request["protobuf_type"], textpb)


def __create_subscriber(response):
    if "zmq" in response:
        zmq_response = response["zmq"]
        return ZmqSubscriberSocket(zmq_response["topic"], zmq_response["address"])

    elif "udp_multicast" in response:
        udp_response = response["udp_multicast"]
        return MulticastUdpSubscriberSocket(udp_response["ip"], udp_response["port"])

    else:
        sys.exit("[error] create_subscriber: no subscriber found from json.")


def create_subscriber_from_response(message):
    return __create_subscriber(message["response"]) if "response" in message else None


if __name__ == "__main__":
    # TODO($ISSUE_N): Fix to work calling from any directory.
    # os.chdir(os.path.dirname(os.path.abspath(__file__)))

    parser = argparse.ArgumentParser()
    parser.add_argument("--input_file", required=True)

    args = parser.parse_args()

    with open(f"inputs/{args.input_file}") as json_input:
        data = json.load(json_input)

        for request in data["requests"]:
            publisher = create_publisher(request)
            message = create_message(request)
            subscriber = create_subscriber_from_response(request)

            serialized_message = serialize(message)

            print(f"publisher type: {type(publisher)}")
            print(f"message type: {type(message)}")
            print(f"subscriber type: {type(subscriber)}")

            # print(f"serialized_message: {type(serialized_message)}")

            # TODO($ISSUE_N): Fix count fps and.
            count = request["count"] if "count" in request else -1
            fps = request["fps"] if "fps" in request else 60

            total_sent = 0
            while total_sent != count:
                publisher.send(serialized_message)
                print(f"serialized_message of '{type(message)}' sent.")

                total_sent += 1
                time.sleep(1 / fps)

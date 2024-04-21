from datetime import datetime as dt
import json
import argparse
import sys
import threading
import time

from network.multicast_udp_publisher_subscriber import (
    MulticastUdpPublisherSocket,
    MulticastUdpSubscriberSocket,
)
from network.pub_sub_mode import PubSubMode
from network.zmq_request_reply import ZmqRequestSocket
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
        print(f'response["zmq"]: {zmq_response}')
        return ZmqSubscriberSocket(zmq_response["topic"], zmq_response["address"])

    elif "udp_multicast" in response:
        udp_response = response["udp_multicast"]
        return MulticastUdpSubscriberSocket(udp_response["ip"], udp_response["port"])

    else:
        sys.exit("[error] create_subscriber: no subscriber found from json.")


def create_subscriber_from_response(message):
    return __create_subscriber(message["response"]) if "response" in message else None


def req_rep_fn(service_id: int):
    msg = parse_from_textpb(
        "protocols.ui.messages.GetVisionChunkRequest",
        """
{
    "header": {
        "request_start": 1,
        "chunk_id": 2,
        "n_chunks": 3,
        "max_duration": 4
    }
}
""",
    )

    socket = ZmqRequestSocket(f"ipc:///tmp/sync-channel{service_id}.ipc")

    value = 0
    while value != -1:
        rcvd = socket.send_and_receive(msg)
        print(f"received {len(rcvd)} bytes from service {service_id}.")
        time.sleep(1)
        value += 1


if __name__ == "__main__":
    # TODO($ISSUE_N): Fix to work calling from any directory.
    # os.chdir(os.path.dirname(os.path.abspath(__file__)))

    parser = argparse.ArgumentParser()
    parser.add_argument("--input_file", required=True)
    parser.add_argument("--req_rep_dummy", default="False")

    args = parser.parse_args()

    with open(f"inputs/{args.input_file}") as json_input:
        data = json.load(json_input)

        for i in range(40):
            time.sleep(1)
            print(f"slept for {i}s.", flush=True)

        for request in data["requests"]:
            threads = []

            # if True:
            #     for service_id in range(5):
            #         thread = threading.Thread(target=req_rep_fn, args=(service_id,))
            #         threads.append(thread)
            #         thread.start()

            publisher = create_publisher(request)
            message = create_message(request)
            subscriber = create_subscriber_from_response(request)

            serialized_message = serialize(message)

            print(f"publisher type: {type(publisher)}")
            print(f"message type: {type(message)}")
            print(f"subscriber type: {type(subscriber)}", flush=True)

            # TODO($ISSUE_N): Fix count fps and.
            count = request["count"] if "count" in request else -1
            fps = request["fps"] if "fps" in request else 60

            total_sent = 0
            diffs = []
            while total_sent != count:
                pub_time = dt.now()
                # print(f'sent {len(serialized_message)} bytes.', flush=True)
                publisher.send(serialized_message)
                if subscriber:
                    received = subscriber.receive(PubSubMode.Wait)
                    if received:
                        rcv_time = dt.now()
                        diffs.append((rcv_time - pub_time).total_seconds() * 1000)

                total_sent += 1
                time.sleep(1 / fps) # commented because it its impacting the benchmark.

            print(diffs)

            for thread in threads:
                thread.join()

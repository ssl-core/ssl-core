from datetime import timedelta, datetime as dt
import json
import argparse
import sys
import time

from network.multicast_udp_publisher_subscriber import (
    MulticastUdpPublisherSocket,
    MulticastUdpSubscriberSocket,
)
from network.pub_sub_mode import PubSubMode
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
        print(f"response[\"zmq\"]: {zmq_response}")
        return ZmqSubscriberSocket(zmq_response["topic"], zmq_response["address"])

    elif "udp_multicast" in response:
        udp_response = response["udp_multicast"]
        return MulticastUdpSubscriberSocket(udp_response["ip"], udp_response["port"])

    else:
        sys.exit("[error] create_subscriber: no subscriber found from json.")


def create_subscriber_from_response(message):
    return __create_subscriber(message["response"]) if "response" in message else None

f = 840
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
            # subscriber = create_subscriber_from_response(request)

            serialized_message = serialize(message)

            print(f"publisher type: {type(publisher)}")
            print(f"message type: {type(message)}")
            # print(f"subscriber type: {type(subscriber)}")

            # TODO($ISSUE_N): Fix count fps and.
            count = request["count"] if "count" in request else -1
            fps = request["fps"] if "fps" in request else 60
            
            total_sent = 0
            diffs = []

            interval = 1 / fps  # Time interval between messages

            start_time = dt.now()
            next_send_time = start_time  # Set the initial time to start sending

            n = -2500

            while total_sent != count:
                now = dt.now()

                # If the current time is greater than or equal to the next scheduled time, send the message
                if now >= next_send_time:
                    # print(f"sending: {total_sent}")
                    pub_time = now
                    publisher.send(serialized_message)

                    # if subscriber:
                    #     received = subscriber.receive(PubSubMode.Wait)
                    #     if received:
                    #         rcv_time = dt.now()
                    #         diffs.append((rcv_time - pub_time).total_seconds() * 1000)

                    total_sent += 1
                    next_send_time += timedelta(seconds=interval)  # Schedule next message send
                    
                    n += 1
                    f += 1
                    
                    # Ensure the value stays within the range [-2500, -500]
                    if n > -500:
                        n = -2500
                    elif n < -2500:
                        n = -500

                    message.detection.robots_blue[0].x = n
                    message.detection.frame_number = f

                    serialized_message = serialize(message)

                    if (total_sent % 60 == 0):
                        print(f"time: {now}, fps: {60 / (now - start_time).total_seconds()}")
                        start_time = now

                # Sleep for a very short time to prevent busy-waiting (optional)
                time.sleep(0.001)  # A short sleep to yield CPU for other processes

            # total_sent = 0
            # diffs = []
            # while total_sent != count:
            #     print(f"sending: {total_sent}")
            #     pub_time = dt.now()
            #     publisher.send(serialized_message)
            #     # if subscriber:
            #     #     received = subscriber.receive(PubSubMode.Wait)
            #     #     if received:
            #     #         rcv_time = dt.now()
            #     #         diffs.append((rcv_time - pub_time).total_seconds() * 1000)

            #     total_sent += 1
            #     time.sleep(1/fps) # commented because it its impacting the benchmark.

            # print(diffs)

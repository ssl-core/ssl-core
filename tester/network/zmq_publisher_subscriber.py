from typing import Union, Optional
from network.pub_sub_mode import PubSubMode
from google.protobuf.message import Message as Protobuf
import zmq


class ZmqPublisherSocket:
    def __init__(self, topic, address, n_threads: int = 1, timeout_ms: int = 3):
        # Create a ZMQ Publisher
        self.context = zmq.Context(n_threads)
        self.timeout_ms = timeout_ms
        self.publisher = self.context.socket(zmq.PUB)
        self.topic = topic.encode()

        # Bind the socket to the given ZMQ address
        self.publisher.bind(address)
        # self.publisher.setsockopt(zmq.SNDTIMEO, self.timeout_ms)

    def send(
        self,
        message: Union[str, bytes, Protobuf],
        mode: PubSubMode = PubSubMode.DontWait,
    ):
        flags = zmq.DONTWAIT if mode == PubSubMode.DontWait else zmq.Flag(0)

        if isinstance(message, str):
            self.publisher.send_multipart([self.topic, message.encode()], flags=flags)
        elif isinstance(message, bytes):
            self.publisher.send_multipart([self.topic, message], flags=flags)
        elif isinstance(message, Protobuf):
            self.publisher.send_multipart(
                [self.topic, message.SerializeToString()], flags=flags
            )
        else:
            raise ValueError(f"Invalid message type: {type(message)}.")


class ZmqSubscriberSocket:
    def __init__(self, topic: str, address: str):
        # Create a ZMQ Subscriber
        self.context = zmq.Context()
        self.subscriber = self.context.socket(zmq.SUB)

        self.subscriber.connect(address)
        self.subscriber.setsockopt_string(zmq.SUBSCRIBE, topic)

    def receive(self, mode=PubSubMode.DontWait) -> Optional[bytes]:
        flags = zmq.DONTWAIT if mode == PubSubMode.DontWait else zmq.Flag(0)
        try:
            topic, message = self.subscriber.recv_multipart(flags=flags)
            # assert topic.decode() == self.topic
            return message
        except zmq.Again:
            return None

from typing import Union
from network.pub_sub_mode import PubSubMode
from google.protobuf.message import Message as Protobuf
import zmq


class ZmqPublisherSocket:
    def __init__(self, n_threads: int = 1, timeout_ms: int = 3):
        # Create a ZMQ Publisher
        self.context = zmq.Context(n_threads)
        self.timeout_ms = timeout_ms
        self.publisher = self.context.socket(zmq.PUB)

    def bind(self, address):
        # Bind the socket to the given ZMQ address
        self.publisher.bind(address)
        self.publisher.setsockopt(zmq.SNDTIMEO, self.timeout_ms)

    def send(
        self,
        topic: str,
        message: Union[str, bytes, Protobuf],
        mode: PubSubMode = PubSubMode.DontWait,
    ):
        flags = zmq.DONTWAIT if mode == PubSubMode.DontWait else zmq.Flag(0)

        if isinstance(message, str):
            self.publisher.send_multipart(
                [topic.encode(), message.encode()], flags=flags
            )
        elif isinstance(message, bytes):
            self.publisher.send_multipart([topic.encode(), message], flags=flags)
        elif isinstance(message, Protobuf):
            self.publisher.send_multipart(
                [topic.encode(), message.SerializeToString()], flags=flags
            )
        else:
            raise ValueError(f"Invalid message type: {type(message)}.")

from typing import Union, Optional
from network.pub_sub_mode import PubSubMode
from google.protobuf.message import Message as Protobuf
import zmq


class ZmqRequestSocket:
    def __init__(self, address, n_threads: int = 1):
        # Create a ZMQ Request
        self.context = zmq.Context(n_threads)
        self.request = self.context.socket(zmq.REQ)

        # Bind the socket to the given ZMQ address
        self.request.connect(address)

    def send_and_receive(self, message: Union[str, bytes, Protobuf]) -> bytes:
        if isinstance(message, str):
            self.request.send(message.encode())
        elif isinstance(message, bytes):
            self.request.send(message)
        elif isinstance(message, Protobuf):
            self.request.send(message.SerializeToString())
        else:
            raise ValueError(f"Invalid message type: {type(message)}.")

        return self.request.recv()


class ZmqReplySocket:
    def __init__(self, address: str):
        # Create a ZMQ Reply
        self.context = zmq.Context()
        self.subscriber = self.context.socket(zmq.REP)

        self.subscriber.bind(address)

    def receive(self):
        return self.subscriber.recv()

    def send(self, message: Union[str, bytes, Protobuf]):
        if isinstance(message, str):
            self.subscriber.send(message.encode())
        elif isinstance(message, bytes):
            self.subscriber.send(message)
        elif isinstance(message, Protobuf):
            self.subscriber.send(message.SerializeToString())
        else:
            raise ValueError(f"Invalid message type: {type(message)}.")

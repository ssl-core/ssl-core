from typing import Optional
from network.pub_sub_mode import PubSubMode
import zmq


class ZmqSubscriberSocket:
    def __init__(self, topic: str, n_threads: int = 1, timeout_ms: int = -1):
        # Create a ZMQ Subscriber
        self.topic = topic
        self.context = zmq.Context(n_threads)
        self.timeout_ms = timeout_ms
        self.subscriber = self.context.socket(zmq.SUB)

    def connect(self, address):
        # Connect the socket to the given ZMQ address
        self.subscriber.connect(address)
        if self.timeout_ms >= 0:
            self.subscriber.setsockopt(zmq.RCVTIMEO, self.timeout_ms)
        self.subscriber.setsockopt(zmq.SUBSCRIBE, self.topic.encode())

    def receive(self, mode) -> Optional[bytes]:
        flags = zmq.DONTWAIT if mode == PubSubMode.DontWait else zmq.Flag(0)

        try:
            topic, message = self.subscriber.recv_multipart(flags=flags)
            assert topic.decode() == self.topic
            return message
        except zmq.Again:
            return None

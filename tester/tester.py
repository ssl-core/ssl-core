import pyzmq
from datetime import datetime
import time
from google.protobuf.text_format import Parse as ParseFromText
from pathlib import Path


class Tester:

    def __init__(self, count, pbType, pbPath, address, topic, response):
        self.count = count
        self.context = pyzmq.Context()
        self.pub = self.context.socket(pyzmq.PUB)
        self.pbType = pbType
        self.pbPath = Path(pbPath).read_text()
        self.address = address
        self.topic = topic
        self.response = response
        self.listDiffTime = []

    def createSocket(self):
        pub = self.context.socket(pyzmq.PUB)
        pub.bind(self.address)

    def measuringTime(init, end):
        t1 = datetime.fromtimestamp(init)
        t2 = datetime.fromtimestamp(end)
        delta = t2 - t1
        diffTime = delta.total_seconds() * 1000
        return [
            "The latency time is: {} ms".format(diffTime),
            diffTime,
        ]

    def sendMessage(self):
        qty = int(self.count)
        proto = self.pbType
        ParseFromText(
            self.pbPath,
            proto,
        )
        message = proto.SerializeToString()
        if qty > 0:
            while qty > 0:
                time.sleep(0.5)
                init_timer = datetime.now().timestamp()
                self.pub.send_multipart([self.topic.encode(), message])
                end_timer = datetime.now().timestamp()
                elapsed_time = measuringTime(init_timer, end_timer)
                self.listDiffTime.append(elapsed_time[1])
                qty = qty - 1

    def latencyMean(self):
        listDiffTime = self.listDiffTime
        latencySum = sum(listDiffTime)
        mean = latencySum / len(listDiffTime)
        return "The mean of the latency is {} ms".format(mean)

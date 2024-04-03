import zmq
from datetime import datetime
import time
from google.protobuf.text_format import Parse as ParseFromText
from pathlib import Path
from protocols.third_party.detection.raw_wrapper_pb2 import SSL_WrapperPacket
from testerLatencyFunctions import latencyMean, measuringTime


class Tester:

    def __init__(self, count, pbType, pbPath, address, topic, response):
        self.count = count
        self.context = zmq.Context()
        self.pub = self.context.socket(zmq.PUB)
        self.PbType = eval(str(pbType))
        self.pbPath = Path(pbPath).read_text()
        self.address = str(address).strip()
        self.topic = str(topic).strip()
        self.response = response
        self.listDiffTime = []

    def createSocket(self):
        self.pub.bind(self.address)

    def sendMessage(self):
        qty = int(self.count)
        proto = self.PbType()
        ParseFromText(
            self.pbPath,
            proto,
        )
        message = proto.SerializeToString()
        while qty > 0:
            time.sleep(0.7)
            initTimer = datetime.now().timestamp()

            self.pub.send_multipart([self.topic.encode(), message])
            # print(message)
            endTimer = datetime.now().timestamp()
            elapsedTime = measuringTime(initTimer, endTimer)
            self.listDiffTime.append(elapsedTime[1])
            qty = qty - 1

    def getLatency(self):
        mean = latencyMean(self.listDiffTime)
        print(
            "The mean of the latency from tester into {} is {} ms".format(
                self.topic, mean
            )
        )

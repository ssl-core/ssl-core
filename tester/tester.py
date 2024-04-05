import zmq
from datetime import datetime
import time
from pathlib import Path
from protocols.third_party.detection.raw_wrapper_pb2 import SSL_WrapperPacket
from utility.latency import measure_mean, measure_time


class Tester:
    def __init__(
        self,
        count,
        pbType,
        pbPath,
        zmqAddress,
        zmqTopic,
        response,
        udpIp,
        udpPort,
        udpInet,
    ):
        self.count = count
        self.context = zmq.Context()
        self.pub = self.context.socket(zmq.PUB)
        self.PbType = pbType
        self.pbPath = Path(pbPath).read_text()
        self.zmqAddress = str(zmqAddress).strip()
        self.zmqTopic = str(zmqTopic).strip()
        self.response = response
        self.listDiffTime = []
        self.udpIp = udpIp
        self.udpPort = udpPort
        self.udpInet = udpInet

    def createZmqSocket(self):
        self.pub.bind(self.zmqAddress)

    def sendMessage(self):
        qty = int(self.count)
        message = pbMessage(self.pbPath, self.PbType)
        while qty > 0:
            time.sleep(0.1)
            initTimer = datetime.now().timestamp()

            self.pub.send_multipart([self.zmqTopic.encode(), message])
            # print(message)
            endTimer = datetime.now().timestamp()
            elapsedTime = measuringTime(initTimer, endTimer)
            self.listDiffTime.append(elapsedTime[1])
            qty = qty - 1

    def getLatency(self):
        mean = measure_mean(self.listDiffTime)
        print(
            "The mean of the latency from tester into {} is {} ms".format(
                self.zmqTopic, mean
            )
        )

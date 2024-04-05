from datetime import datetime
from pathlib import Path
from protocols.third_party.detection.raw_wrapper_pb2 import SSL_WrapperPacket
from google.protobuf.text_format import Parse as ParseFromText


def measuringTime(init, end):
    t1 = datetime.fromtimestamp(init)
    t2 = datetime.fromtimestamp(end)
    delta = t2 - t1
    diffTime = delta.total_seconds() * 1000
    return [
        "The latency time is: {} ms".format(diffTime),
        diffTime,
    ]


def latencyMean(latencyList):
    listDiffTime = latencyList
    latencySum = sum(listDiffTime)
    mean = latencySum / len(listDiffTime)
    return mean


def pbMessage(pbPathValue, pbType):
    pbPath = Path(pbPathValue).read_text()
    PbType = eval(str(pbType))
    proto = PbType()
    ParseFromText(
        pbPath,
        proto,
    )
    message = proto.SerializeToString()

    return message

#!/usr/local/bin/python3
from google.protobuf.text_format import Parse as ParseFromText
from google.protobuf.json_format import Parse as ParseFromJson

from protocols.third_party.detection.raw_wrapper_pb2 import SSL_WrapperPacket

import zmq
from datetime import datetime
import sys
from pathlib import Path
import time


context = zmq.Context()
arg_range = int(sys.argv[1])

# ler o json com as infos dos serviços que serão testados
# fileName = "tester-data.json"
# read_json_data = open(fileName, "r")

pb_text_path = "message.textpb"
pb_text = Path(pb_text_path).read_text()


pub = context.socket(zmq.PUB)
address = "ipc:///tmp/vision-service"
pub.bind(address)
list_diff_time = []

ProtobufType = eval("SSL_WrapperPacket")


def measure_time(init, end):
    t1 = datetime.fromtimestamp(init)

    t2 = datetime.fromtimestamp(end)
    delta = t2 - t1
    diff_time = delta.total_seconds() * 1000
    return [
        "The latency time is: {} ms".format(diff_time),
        diff_time,
    ]


def latency_mean(list_latency):
    total_latency = sum(list_latency)
    mean = total_latency / len(list_latency)
    return "The mean of the latency is {} ms".format(mean)


for request in range(arg_range):
    proto = ProtobufType()
    ParseFromText(
        pb_text,
        proto,
    )
    msg = proto.SerializeToString()
    time.sleep(1)

    # init timer
    init_timer = datetime.now().timestamp()
    pub.send_multipart(["robocin".encode(), msg])

    # end timer
    end_timer = datetime.now().timestamp()
    elapsed_time = measure_time(init_timer, end_timer)
    list_diff_time.append(elapsed_time[1])


mean = latency_mean(list_diff_time)
print(mean)

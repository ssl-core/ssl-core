#!/usr/local/bin/python3
from google.protobuf.text_format import Parse as ParseFromText
from google.protobuf.json_format import Parse as ParseFromJson

from protocols.common.geometry_pb2 import Point2Df
import zmq

pt = Point2Df()

ParseFromText(
    """
      x: 1.0
      y: 2.0
    """,
    pt,
)

print(pt)

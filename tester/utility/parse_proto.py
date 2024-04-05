from google.protobuf.text_format import Parse as ParseFromText


def serialize_message(type_name, textpb):
    ProtobufClass = eval(type_name)
    pb = ProtobufClass()
    ParseFromText(
        textpb,
        pb,
    )
    return pb.SerializeToString()

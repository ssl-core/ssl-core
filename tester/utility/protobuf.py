import os
import protocols
import importlib

from google.protobuf.text_format import Parse as ParseFromText

__PYTHON_EXTENSION = ".py"
__PROTOBUF_MODULE_SUFFIX = "_pb2"


def __find_protocols_packages(directory):
    pb_files = []
    for root, _, files in os.walk(directory):
        for file in files:
            if file.endswith(__PROTOBUF_MODULE_SUFFIX + __PYTHON_EXTENSION):
                pb_files.append(os.path.relpath(os.path.join(root, file), directory))

    return [file.replace("/", ".")[: -len(__PYTHON_EXTENSION)] for file in pb_files]


for module in __find_protocols_packages(os.path.dirname(protocols.__path__[0])):
    importlib.import_module(module)


def parse_from_textpb(type: str, textpb: str):
    module, protobuf = type[: type.rfind(".")], type[type.rfind(".") + 1 :]

    # protobufs are generated with '_pb2' suffix.
    if not module.endswith(__PROTOBUF_MODULE_SUFFIX):
        module += __PROTOBUF_MODULE_SUFFIX

    clazz = eval(f"{module}.{protobuf}")
    protobuf = clazz()

    ParseFromText(textpb, protobuf)

    return protobuf


def serialize(protobuf) -> bytes:
    return protobuf.SerializeToString()

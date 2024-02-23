import os

def get_proto_files():
    proto_files = []
    for root, dirs, files in os.walk('protocols'):
        for file in files:
            if file.endswith('.proto'):
                proto_files.append(os.path.join(root, file))
    return proto_files

def compile_proto_files():
    proto_files = get_proto_files()
    for proto_file in proto_files:
        print('Compiling', proto_file)
        os.system('protoc --python_out=. -Iprotocols ' + proto_file)

compile_proto_files()

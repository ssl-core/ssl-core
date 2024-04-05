import zmq


if __name__ == "__main__":
    context = zmq.Context()
    socket = context.socket(zmq.SUB)

    address = "ipc:///tmp/vision-service"
    socket.connect(address)
    socket.setsockopt_string(zmq.SUBSCRIBE, "vision-tester")

    count = 0
    while True:
        topic, message = socket.recv_multipart()
        count = count + 1
        print(f"Received request: {message} from {topic} for {count} times")

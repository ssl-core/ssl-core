import zmq

context = zmq.Context()
sub = context.socket(zmq.SUB)
address = "ipc:///tmp/vision-service"
sub.connect(address)
sub.setsockopt_string(zmq.SUBSCRIBE, "vision-tester")

count = 0
while True:
    topic, message = sub.recv_multipart()
    count = count + 1
    print(f"Received request: {message} from {topic} for {count} times")

import pyzmq

context = pyzmq.Context()
sub = context.socket(pyzmq.SUB)
address = "ipc:///tmp/vision-service"
sub.connect(address)
sub.setsockopt_string(pyzmq.SUBSCRIBE, "robocin")

count = 0
while True:

    topic, message = sub.recv_multipart()
    count = count + 1
    print(f"Received request: {message} from {topic} for {count} times")

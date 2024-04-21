from network.zmq_request_reply import ZmqReplySocket

if __name__ == "__main__":
    socket = ZmqReplySocket("ipc:///tmp/test.ipc")

    while True:
        message = socket.receive()
        print(f"received {message}.")

        socket.send(message)
        print(f"sent {message}.")

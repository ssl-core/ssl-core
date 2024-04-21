from network.zmq_request_reply import ZmqRequestSocket
import time

if __name__ == "__main__":
    socket = ZmqRequestSocket("ipc:///tmp/test.ipc")

    while True:
        print(f"received back {socket.send_and_receive(b"hello")}")
        time.sleep(1)

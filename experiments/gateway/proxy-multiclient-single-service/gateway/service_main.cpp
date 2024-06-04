#include <zmq.h>
#include <zmq.hpp>
#include <zmq_addon.hpp>

int main() {
  zmq::context_t context(1);
  zmq::socket_t socket(context, ZMQ_DEALER);
  socket.connect("ipc:///tmp/gateway-backend.ipc");

  while (true) {
    zmq::multipart_t messages;
    messages.recv(socket);
    messages.send(socket);
  }

  return 0;
}

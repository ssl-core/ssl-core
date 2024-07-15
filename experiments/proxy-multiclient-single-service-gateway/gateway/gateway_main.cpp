#include <zmq.hpp>

int main() {
  zmq::context_t context(1);
  zmq::socket_t frontend(context, ZMQ_ROUTER);
  frontend.bind("ipc:///tmp/.ssl-core/gateway-frontend.ipc");

  zmq::socket_t backend(context, ZMQ_DEALER);
  backend.bind("ipc:///tmp/.ssl-core/gateway-backend.ipc");

  zmq::proxy(frontend, backend);

  return 0;
}

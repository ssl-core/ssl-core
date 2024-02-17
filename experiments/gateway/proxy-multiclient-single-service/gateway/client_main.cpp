#include <format>
#include <iostream>
#include <zmq.hpp>
#include <zmq_addon.hpp>

constexpr int kRequest = 10;
constexpr int kSleep = 2;

int main(int argc, char* argv[]) {
  auto args = std::span(argv, static_cast<size_t>(argc));
  if (args.size() != 2) {
    std::cout << "Add a client identifier.";
    return -1;
  }

  zmq::context_t context(1);
  zmq::socket_t socket(context, ZMQ_REQ);
  socket.connect("ipc:///tmp/gateway-frontend.ipc");

  for (int i = 0; i < kRequest; ++i) {
    zmq::message_t request{std::format("{}: Hello {}!", args[1], i)};
    socket.send(request, zmq::send_flags::none);

    zmq::message_t reply;
    auto _ = socket.recv(reply, zmq::recv_flags::none);
    std::cout << "Response in " << reply.to_string() << "\n";

    sleep(kSleep);
  }

  return 0;
}

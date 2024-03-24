#ifndef ROBOCIN_NETWORK_ZMQ_REQUEST_REPLY_SOCKET_H
#define ROBOCIN_NETWORK_ZMQ_REQUEST_REPLY_SOCKET_H

#include <zmq.h>
#include <zmq.hpp>

namespace robocin {

template <class ZmqSocket, class ZmqContext, zmq::socket_type SocketType>
class IZmqRequestReplySocket {
 public:
  using receive_type = std::vector<zmq::message_t>;

  explicit IZmqRequestReplySocket(int n_threads = 1) :
      context_(n_threads),
      socket_(context_, SocketType) {}

  void bind(std::string_view address) { socket_.bind(std::string{address}); }
  void connect(std::string_view address) { socket_.connect(std::string{address}); }

  std::string receive() {
    zmq::message_t message;
    auto _ = socket_.recv(message, zmq::recv_flags::none);
    return message.to_string();
  }

  void send(std::string_view message) {
    zmq::message_t zmq_message{message};
    socket_.send(zmq_message, zmq::send_flags::none);
  }

  void close() { socket_.close(); }

  [[nodiscard]] int fd() const { return socket_.get(zmq::sockopt::fd); }

 private:
  ZmqContext context_;
  ZmqSocket socket_;
};

using ZmqRequestSocket
    = IZmqRequestReplySocket<zmq::socket_t, zmq::context_t, zmq::socket_type::req>;
using ZmqReplySocket = IZmqRequestReplySocket<zmq::socket_t, zmq::context_t, zmq::socket_type::rep>;

} // namespace robocin

#endif // ROBOCIN_NETWORK_ZMQ_REQUEST_REPLY_SOCKET_H

#ifndef GATEWAY_NETWORK_ZMQ_ROUTER_DEALER_SOCKET_H
#define GATEWAY_NETWORK_ZMQ_ROUTER_DEALER_SOCKET_H

#include <utility>
#include <zmq.h>
#include <zmq.hpp>
#include <zmq_addon.hpp>

namespace gateway {

template <class ZmqSocket, class ZmqContext, zmq::socket_type SocketType>
class IZmqRouterDealerSocket {
 public:
  using receive_type = std::vector<zmq::message_t>;

  explicit IZmqRouterDealerSocket(int n_threads = 1) :
      context_(n_threads),
      socket_(context_, SocketType) {}

  void bind(std::string_view address) { socket_.bind(std::string{address}); }
  void connect(std::string_view address) { socket_.connect(std::string{address}); }

  receive_type receive() {
    receive_type messages;
    // TODO($ISSUE_N): Define recv_flags.
    auto _ = zmq::recv_multipart(socket_, std::back_inserter(messages), zmq::recv_flags::dontwait);
    return messages;
  }

  template <class R>
  void send(R&& messages) {
    // TODO($ISSUE_N): Define send_flags.
    zmq::send_multipart(socket_, std::forward<R>(messages), zmq::send_flags::dontwait);
  }

  void close() { socket_.close(); }

  [[nodiscard]] int fd() const { return socket_.get(zmq::sockopt::fd); }

  ZmqSocket& socket() { return socket_; }

 private:
  ZmqContext context_;
  ZmqSocket socket_;
};

using ZmqRouterSocket
    = IZmqRouterDealerSocket<zmq::socket_t, zmq::context_t, zmq::socket_type::router>;
using ZmqDealerSocket
    = IZmqRouterDealerSocket<zmq::socket_t, zmq::context_t, zmq::socket_type::dealer>;

} // namespace gateway

#endif // GATEWAY_NETWORK_ZMQ_ROUTER_DEALER_SOCKET_H

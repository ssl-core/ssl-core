#ifndef VISION_NETWORK_ZMQ_PUBLISHER_SOCKET_H
#define VISION_NETWORK_ZMQ_PUBLISHER_SOCKET_H
#include "vision/network/socket.h"

#include <string>
#include <zmq.h>
#include <zmq.hpp>

namespace vision {
class ZmqPublisherSocket : public ISocketSender {
 public:
  explicit ZmqPublisherSocket(int n_threads, const std::string& address);

  // TODO(aalmds): review user cases for topics.
  void send(std::string_view topic, std::string_view message) override;

  void close() override;

 private:
  zmq::context_t context_;
  zmq::socket_t socket_;
};
} // namespace vision

#endif // VISION_NETWORK_ZMQ_PUBLISHER_SOCKET_H

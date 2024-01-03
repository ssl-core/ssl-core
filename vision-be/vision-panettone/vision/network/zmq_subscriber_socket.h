#ifndef VISION_NETWORK_ZMQ_SUBSCRIBER_SOCKET_H
#define VISION_NETWORK_ZMQ_SUBSCRIBER_SOCKET_H

#include "vision/network/socket.h"

#include <string>
#include <string_view>
#include <zmq.h>
#include <zmq.hpp>

namespace vision {

class ZmqSubscriberSocket : public ISocketReceiver {
 public:
  ZmqSubscriberSocket(std::string_view topic, int n_threads, const std::string& address);

  std::string receive() override;

  void close() override;

  [[nodiscard]] int fileDescriptor() const override;

 private:
  zmq::context_t context_;
  zmq::socket_t socket_;
  std::string topic_;
  int file_descriptor_{};
};

} // namespace vision

#endif // VISION_NETWORK_ZMQ_SUBSCRIBER_SOCKET_H

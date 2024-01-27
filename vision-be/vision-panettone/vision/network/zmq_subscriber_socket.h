#ifndef VISION_NETWORK_ZMQ_SUBSCRIBER_SOCKET_H
#define VISION_NETWORK_ZMQ_SUBSCRIBER_SOCKET_H

#include <string>
#include <zmq.h>
#include <zmq.hpp>

namespace vision {

class ZmqSubscriberSocket {
 public:
  struct Datagram;

  explicit ZmqSubscriberSocket(int n_threads);

  void connect(const std::string& address, std::span<const std::string> topics);

  Datagram receive();

  void close();

  [[nodiscard]] int fileDescriptor() const;

 private:
  zmq::context_t context_;
  zmq::socket_t socket_;
  int file_descriptor_{};
};

struct ZmqSubscriberSocket::Datagram {
  std::string topic;
  std::string message;
};

} // namespace vision

#endif // VISION_NETWORK_ZMQ_SUBSCRIBER_SOCKET_H

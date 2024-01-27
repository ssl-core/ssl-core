#ifndef VISION_NETWORK_ZMQ_PUBLISHER_SOCKET_H
#define VISION_NETWORK_ZMQ_PUBLISHER_SOCKET_H

#include <string>
#include <string_view>
#include <zmq.h>
#include <zmq.hpp>

namespace vision {

class ZmqPublisherSocket {
 public:
  explicit ZmqPublisherSocket(int n_threads);

  void bind(const std::string& address);
  void send(std::string_view topic, std::string_view message);
  void close();

 private:
  zmq::context_t context_;
  zmq::socket_t socket_;
};

} // namespace vision

#endif // VISION_NETWORK_ZMQ_PUBLISHER_SOCKET_H

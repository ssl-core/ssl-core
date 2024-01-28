#ifndef VISION_NETWORK_ZMQ_PUBLISHER_SOCKET_H
#define VISION_NETWORK_ZMQ_PUBLISHER_SOCKET_H

#include <string_view>
#include <zmq.h>
#include <zmq.hpp>

namespace vision {

class IZmqPublisherSocket {
 public:
  virtual ~IZmqPublisherSocket() = default;

  virtual void bind(std::string_view address) = 0;
  virtual void send(std::string_view topic, std::string_view message) = 0;
  virtual void close() = 0;
};

class ZmqPublisherSocket : public IZmqPublisherSocket {
 public:
  explicit ZmqPublisherSocket(int n_threads = 1);

  void bind(std::string_view address) override;
  void send(std::string_view topic, std::string_view message) override;
  void close() override;

 private:
  zmq::context_t context_;
  zmq::socket_t socket_;
};

} // namespace vision

#endif // VISION_NETWORK_ZMQ_PUBLISHER_SOCKET_H

#ifndef VISION_NETWORK_ZMQ_SUBSCRIBER_SOCKET_H
#define VISION_NETWORK_ZMQ_SUBSCRIBER_SOCKET_H

#include <string>
#include <zmq.h>
#include <zmq.hpp>

namespace vision {

struct ZmqDatagram {
  std::string topic;
  std::string message;
};

class IZmqSubscriberSocket {
 public:
  virtual ~IZmqSubscriberSocket() = default;

  virtual void connect(std::string_view address, std::span<std::string_view> topics) = 0;
  virtual ZmqDatagram receive() = 0;
  virtual void close() = 0;

  [[nodiscard]] virtual int fd() const = 0;
};

class ZmqSubscriberSocket : public IZmqSubscriberSocket {
 public:
  using receive_type = ZmqDatagram;

  explicit ZmqSubscriberSocket(int n_threads = 1);

  void connect(std::string_view address, std::span<std::string_view> topics) override;
  receive_type receive() override;
  void close() override;

  [[nodiscard]] int fd() const override;

 private:
  zmq::context_t context_;
  zmq::socket_t socket_;
  int fd_{};
};

} // namespace vision

#endif // VISION_NETWORK_ZMQ_SUBSCRIBER_SOCKET_H

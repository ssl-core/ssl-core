#ifndef INES_ZMQ_COMMON_PUBLISH_SUBSCRIBE_H
#define INES_ZMQ_COMMON_PUBLISH_SUBSCRIBE_H

#include <string>
#include <string_view>

#include "mock/common/publish_subscribe.h"

#include <zmq.hpp>

namespace ines {

class ZmqPublisher : public ITopicPublisher {
 public:
  explicit ZmqPublisher(int n_threads);
  ZmqPublisher();
  void bind(std::string_view address) override;
  void send(std::string_view topic, PubSubMode mode, std::string_view message) override;

 private:
  static constexpr int kSocketType = ZMQ_PUB;

  ::zmq::context_t context_;
  ::zmq::socket_t publisher_;
};

class ZmqSubscriber : public ITopicSubscriber {
 public:
  ZmqSubscriber(std::string_view topic, int n_threads);
  explicit ZmqSubscriber(std::string_view topic);
  void connect(std::string_view address) override;
  bool receive(PubSubMode mode, std::string& result) override;

 private:
  static constexpr int kSocketType = ZMQ_SUB;

  ::zmq::context_t context_;
  ::zmq::socket_t subscriber_;
};

} // namespace ines

#endif // INES_ZMQ_COMMON_PUBLISH_SUBSCRIBE_H

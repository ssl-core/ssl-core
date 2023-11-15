#include "mock/common/zmq_publish_subscribe.h"

namespace ines {

namespace {

using ZmqSendFlags = zmq::send_flags;
using ZmqRecvFlags = zmq::recv_flags;

constexpr int kSingleThread = 1;

} // namespace

// ZmqPublisher ------------------------------------------------------------------------------------
ZmqPublisher::ZmqPublisher(int n_threads) :
    context_(n_threads),
    publisher_(context_, kSocketType) {}

ZmqPublisher::ZmqPublisher() : ZmqPublisher(kSingleThread) {}

void ZmqPublisher::bind(std::string_view address) { publisher_.bind(address.data()); }

void ZmqPublisher::send(std::string_view topic, PubSubMode mode, std::string_view message) {
  ZmqSendFlags flags = mode == PubSubMode::DontWait ? ZmqSendFlags::dontwait : ZmqSendFlags::none;

  zmq::message_t zmq_topic(topic);
  publisher_.send(zmq_topic, zmq::send_flags::sndmore);

  zmq::message_t zmq_message(message);
  publisher_.send(zmq_message, flags);
}

// ZmqSubscriber -----------------------------------------------------------------------------------
ZmqSubscriber::ZmqSubscriber(std::string_view topic, int n_threads) :
    ITopicSubscriber(topic),
    context_(n_threads),
    subscriber_(context_, kSocketType) {}

ZmqSubscriber::ZmqSubscriber(std::string_view topic) : ZmqSubscriber(topic, kSingleThread) {}

void ZmqSubscriber::connect(std::string_view address) {
  subscriber_.connect(address.data());
  subscriber_.set(zmq::sockopt::subscribe, topic());
}

bool ZmqSubscriber::receive(PubSubMode mode, std::string& result) {
  ZmqRecvFlags flags = mode == PubSubMode::DontWait ? ZmqRecvFlags::dontwait : ZmqRecvFlags::none;

  if (zmq::message_t zmq_topic; subscriber_.recv(zmq_topic, flags)) {
    if (zmq::message_t zmq_result; subscriber_.recv(zmq_result, flags)) {
      result.assign(zmq_result.to_string());
      return true;
    }
  }
  return false;
}

} // namespace ines

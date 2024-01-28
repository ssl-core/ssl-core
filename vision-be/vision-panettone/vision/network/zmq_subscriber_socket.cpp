#include "vision/network/zmq_subscriber_socket.h"

namespace vision {

ZmqSubscriberSocket::ZmqSubscriberSocket(int n_threads) :
    context_(n_threads),
    socket_(context_, ZMQ_SUB),
    fd_(socket_.get(zmq::sockopt::fd)) {}

void ZmqSubscriberSocket::connect(std::string_view address, std::span<std::string_view> topics) {
  socket_.connect(std::string{address});
  for (const auto& topic : topics) {
    socket_.set(zmq::sockopt::subscribe, topic);
  }
}

void ZmqSubscriberSocket::close() { socket_.close(); }

int ZmqSubscriberSocket::fd() const { return fd_; }

ZmqSubscriberSocket::receive_type ZmqSubscriberSocket::receive() {
  if (zmq::message_t zmq_topic; socket_.recv(zmq_topic, zmq::recv_flags::dontwait)) {
    if (zmq::message_t zmq_result; socket_.recv(zmq_result, zmq::recv_flags::dontwait)) {
      ZmqDatagram datagram;
      datagram.topic.assign(zmq_topic.to_string());
      datagram.message.assign(zmq_result.to_string());
      return datagram;
    }
  }

  return {};
}

} // namespace vision

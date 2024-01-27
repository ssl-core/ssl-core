#include "vision/network/zmq_subscriber_socket.h"

namespace vision {

ZmqSubscriberSocket::ZmqSubscriberSocket(int n_threads) :
    context_(n_threads),
    socket_(context_, ZMQ_SUB) {
  size_t fd_size = sizeof(file_descriptor_);
  socket_.getsockopt(ZMQ_FD, &file_descriptor_, &fd_size);
}

void ZmqSubscriberSocket::connect(const std::string& address, std::span<const std::string> topics) {
  socket_.connect(address);
  for (const auto& topic : topics) {
    socket_.set(zmq::sockopt::subscribe, topic);
  }
}

void ZmqSubscriberSocket::close() { zmq_close(socket_); }

int ZmqSubscriberSocket::fileDescriptor() const { return file_descriptor_; }

ZmqSubscriberSocket::Datagram ZmqSubscriberSocket::receive() {
  Datagram datagram;

  if (zmq::message_t zmq_topic; socket_.recv(zmq_topic, zmq::recv_flags::dontwait)) {
    if (zmq::message_t zmq_result; socket_.recv(zmq_result, zmq::recv_flags::dontwait)) {
      datagram.topic.assign(zmq_topic.to_string());
      datagram.message.assign(zmq_result.to_string());
      return datagram;
    }
  }

  return {};
}
} // namespace vision

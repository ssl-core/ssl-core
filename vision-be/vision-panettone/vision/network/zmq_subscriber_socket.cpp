#include "vision/network/zmq_subscriber_socket.h"

namespace vision {

ZmqSubscriberSocket::ZmqSubscriberSocket(std::string_view topic,
                                         int n_threads,
                                         const std::string& address) :
    topic_(topic),
    context_(n_threads),
    socket_(context_, ZMQ_SUB) {
  socket_.connect(address);
  socket_.set(zmq::sockopt::subscribe, topic_);
  size_t fd_size = sizeof(file_descriptor_);
  // socket_.get(zmq::sockopt::fd, file_descriptor_);
  socket_.getsockopt(ZMQ_FD, &file_descriptor_, &fd_size);
}

void ZmqSubscriberSocket::close() { zmq_close(socket_); }

int ZmqSubscriberSocket::fileDescriptor() const { return file_descriptor_; }

std::string ZmqSubscriberSocket::receive() {
  std::string message;

  if (zmq::message_t zmq_topic; socket_.recv(zmq_topic, zmq::recv_flags::dontwait)) {
    if (zmq::message_t zmq_result; socket_.recv(zmq_result, zmq::recv_flags::dontwait)) {
      message.assign(zmq_result.to_string());
    }
  }
  return message;
}
} // namespace vision

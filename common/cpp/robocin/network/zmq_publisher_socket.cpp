#include "robocin/network/zmq_publisher_socket.h"

namespace vision {

ZmqPublisherSocket::ZmqPublisherSocket(int n_threads) :
    context_(n_threads),
    socket_(context_, ZMQ_PUB) {}

void ZmqPublisherSocket::bind(std::string_view address) { socket_.bind(std::string{address}); }

void ZmqPublisherSocket::send(std::string_view topic, std::string_view message) { // NOLINT
  zmq::message_t zmq_topic(topic);
  socket_.send(zmq_topic, zmq::send_flags::sndmore);

  zmq::message_t zmq_message(message);
  socket_.send(zmq_message, zmq::send_flags::dontwait);
}

void ZmqPublisherSocket::close() { zmq_close(socket_); }

} // namespace vision

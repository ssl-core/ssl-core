#ifndef VISION_NETWORK_ZMQ_SENDER_H
#define VISION_NETWORK_ZMQ_SENDER_H

#include "robocin/network/zmq_publisher_socket.h"
#include "vision/network/isender_communication.h"

#include <string_view>
#include <zmq.h>
#include <zmq.hpp>
#include <zmq_addon.hpp>

namespace vision {

class ZMQSenderCommunication : public ISenderCommunication {
 public:
  void bind(std::string_view address, std::string_view topic) override {
    socket_.bind(address);
    topic_ = topic;
  }

  void send(const protocols::vision::Frame& frame) override {
    std::string message;
    frame.SerializeToString(&message);
    socket_.send(topic_, message);
    std::cout << "Sending message on topic " << topic_ << "\n";
  }

  void close() { socket_.close(); }

 private:
  std::string topic_;
  robocin::ZmqPublisherSocket socket_;
};

} // namespace vision

#endif // VISION_NETWORK_ZMQ_SENDER_H

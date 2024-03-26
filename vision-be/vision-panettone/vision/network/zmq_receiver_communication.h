#ifndef VISION_NETWORK_ZMQ_RECEIVER_H
#define VISION_NETWORK_ZMQ_RECEIVER_H

#include "robocin/network/zmq_subscriber_socket.h"
#include "vision/network/ireceiver_communication.h"

#include <string_view>
#include <zmq.h>
#include <zmq.hpp>
#include <zmq_addon.hpp>

namespace vision {

class ZMQReceiverCommunication : public IReceiverCommunication {
 public:
  void connect(std::string_view address) override {
    std::string_view topic;
    socket_.connect(address, std::span{&topic, 1});
  }

  robocin::ZmqDatagram receive() override { return socket_.receive(); }

  void close() { socket_.close(); }

 private:
  robocin::ZmqSubscriberSocket socket_;
};

} // namespace vision

#endif // VISION_NETWORK_ZMQ_RECEIVER_H

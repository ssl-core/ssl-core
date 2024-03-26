#ifndef VISION_NETWORK_IRECEIVER_COMMUNICATION_H
#define VISION_NETWORK_IRECEIVER_COMMUNICATION_H

#include <robocin/network/zmq_subscriber_socket.h>

namespace vision {

class IReceiverCommunication {
 public:
  IReceiverCommunication() = default;
  virtual ~IReceiverCommunication() = default;
  IReceiverCommunication(const IReceiverCommunication&) = default;
  IReceiverCommunication& operator=(const IReceiverCommunication&) = default;
  IReceiverCommunication(IReceiverCommunication&&) = default;
  IReceiverCommunication& operator=(IReceiverCommunication&&) = default;

  // Receive data from the host.
  virtual robocin::ZmqDatagram receive() = 0;

  // Connect to the host.
  // TODO($ISSUE_N): Add needed connect parameters.
  virtual void connect(std::string_view address) = 0;
};

} // namespace vision

#endif // VISION_NETWORK_IRECEIVER_COMMUNICATION_H

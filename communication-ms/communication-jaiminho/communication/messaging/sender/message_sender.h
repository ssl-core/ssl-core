#ifndef COMMUNICATION_MESSAGING_SENDER_MESSAGE_SENDER_H
#define COMMUNICATION_MESSAGING_SENDER_MESSAGE_SENDER_H

#include "robocin/network/zmq_publisher_socket.h"
#include "robocin/network/udp_multicast_socket_sender.h"

#include <protocols/communication/robot_info.pb.h>

namespace communication {

class IMessageSender {
 public:
  IMessageSender() = default;

  IMessageSender(const IMessageSender&) = delete;
  IMessageSender& operator=(const IMessageSender&) = delete;
  IMessageSender(IMessageSender&&) = default;
  IMessageSender& operator=(IMessageSender&&) = default;

  virtual ~IMessageSender() = default;

  virtual void send(const ::protocols::communication::RobotInfo& robot_command) = 0;
};

class MessageSender : public IMessageSender {
 public:
  explicit MessageSender(std::unique_ptr<::robocin::IZmqPublisherSocket> communication_socket,
                         std::unique_ptr<::robocin::IUdpMulticastSocketSender> robot_socket);

  void send(const ::protocols::communication::RobotInfo& robot_commnad) override;

 private:
  std::unique_ptr<::robocin::IZmqPublisherSocket> communication_socket_;
  std::unique_ptr<::robocin::IUdpMulticastSocketSender> robot_socket_;
};

} // namespace communication

#endif // COMMUNICATION_MESSAGING_SENDER_MESSAGE_SENDER_H

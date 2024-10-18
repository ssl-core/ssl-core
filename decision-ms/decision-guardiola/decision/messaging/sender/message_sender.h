#ifndef DECISION_MESSAGING_SENDER_MESSAGE_SENDER_H
#define DECISION_MESSAGING_SENDER_MESSAGE_SENDER_H

#include "robocin/network/zmq_publisher_socket.h"

#include <protocols/decision/decision.pb.h>

namespace decision {

class IMessageSender {
 public:
  IMessageSender() = default;

  IMessageSender(const IMessageSender&) = delete;
  IMessageSender& operator=(const IMessageSender&) = delete;
  IMessageSender(IMessageSender&&) = default;
  IMessageSender& operator=(IMessageSender&&) = default;

  virtual ~IMessageSender() = default;

  virtual void send(const ::protocols::decision::Decision& decision) = 0;
};

class MessageSender : public IMessageSender {
 public:
  explicit MessageSender(std::unique_ptr<::robocin::IZmqPublisherSocket> detection_socket);

  void send(const ::protocols::decision::Decision& decision) override;

 private:
  std::unique_ptr<::robocin::IZmqPublisherSocket> decision_socket_;
};

} // namespace decision

#endif // DECISION_MESSAGING_SENDER_MESSAGE_SENDER_H

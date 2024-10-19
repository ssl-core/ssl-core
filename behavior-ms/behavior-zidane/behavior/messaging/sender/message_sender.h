#ifndef BEHAVIOR_MESSAGING_SENDER_MESSAGE_SENDER_H
#define BEHAVIOR_MESSAGING_SENDER_MESSAGE_SENDER_H

#include "robocin/network/zmq_publisher_socket.h"

#include <protocols/behavior/behavior_unification.pb.h>

namespace behavior {

class IMessageSender {
 public:
  IMessageSender() = default;

  IMessageSender(const IMessageSender&) = delete;
  IMessageSender& operator=(const IMessageSender&) = delete;
  IMessageSender(IMessageSender&&) = default;
  IMessageSender& operator=(IMessageSender&&) = default;

  virtual ~IMessageSender() = default;

  virtual void send(const ::protocols::behavior::unification::Behavior& behavior) = 0;
};

class MessageSender : public IMessageSender {
 public:
  explicit MessageSender(std::unique_ptr<::robocin::IZmqPublisherSocket> behavior_socket);

  void send(const ::protocols::behavior::unification::Behavior& behavior) override;

 private:
  std::unique_ptr<::robocin::IZmqPublisherSocket> behavior_socket_;
};

} // namespace behavior

#endif // BEHAVIOR_MESSAGING_SENDER_MESSAGE_SENDER_H

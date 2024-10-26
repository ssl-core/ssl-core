#ifndef NAVIGATION_MESSAGING_SENDER_MESSAGE_SENDER_H
#define NAVIGATION_MESSAGING_SENDER_MESSAGE_SENDER_H

#include "robocin/network/zmq_publisher_socket.h"

#include <protocols/navigation/navigation.pb.h>

namespace navigation {

class IMessageSender {
 public:
  IMessageSender() = default;

  IMessageSender(const IMessageSender&) = delete;
  IMessageSender& operator=(const IMessageSender&) = delete;
  IMessageSender(IMessageSender&&) = default;
  IMessageSender& operator=(IMessageSender&&) = default;

  virtual ~IMessageSender() = default;

  virtual void send(const ::protocols::navigation::Navigation& navigation) = 0;
};

class MessageSender : public IMessageSender {
 public:
  explicit MessageSender(std::unique_ptr<::robocin::IZmqPublisherSocket> navigation_socket);

  void send(const ::protocols::navigation::Navigation& navigation) override;

 private:
  std::unique_ptr<::robocin::IZmqPublisherSocket> navigation_socket_;
};

} // namespace navigation

#endif // NAVIGATION_MESSAGING_SENDER_MESSAGE_SENDER_H

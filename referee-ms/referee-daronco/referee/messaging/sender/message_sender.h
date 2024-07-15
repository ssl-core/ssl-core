#ifndef REFEREE_MESSAGING_SENDER_MESSAGE_SENDER_H
#define REFEREE_MESSAGING_SENDER_MESSAGE_SENDER_H

#include "robocin/network/zmq_publisher_socket.h"

#include <protocols/referee/game_status.pb.h>

namespace referee {

class IMessageSender {
 public:
  IMessageSender() = default;

  IMessageSender(const IMessageSender&) = delete;
  IMessageSender& operator=(const IMessageSender&) = delete;
  IMessageSender(IMessageSender&&) = default;
  IMessageSender& operator=(IMessageSender&&) = default;

  virtual ~IMessageSender() = default;

  virtual void send(const ::protocols::referee::GameStatus& game_status) = 0;
};

class MessageSender : public IMessageSender {
 public:
  explicit MessageSender(std::unique_ptr<::robocin::IZmqPublisherSocket> detection_socket);

  void send(const ::protocols::referee::GameStatus& game_status) override;

 private:
  std::unique_ptr<::robocin::IZmqPublisherSocket> detection_socket_;
};

} // namespace referee

#endif // REFEREE_MESSAGING_SENDER_MESSAGE_SENDER_H

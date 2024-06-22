#ifndef PERCEPTION_MESSAGING_SENDER_MESSAGE_SENDER_H
#define PERCEPTION_MESSAGING_SENDER_MESSAGE_SENDER_H

#include "robocin/network/zmq_publisher_socket.h"

#include <protocols/perception/detection.pb.h>

namespace perception {

class IMessageSender {
 public:
  IMessageSender() = default;

  IMessageSender(const IMessageSender&) = delete;
  IMessageSender& operator=(const IMessageSender&) = delete;
  IMessageSender(IMessageSender&&) = default;
  IMessageSender& operator=(IMessageSender&&) = default;

  virtual ~IMessageSender() = default;

  virtual void send(const ::protocols::perception::Detection& detection) = 0;
  virtual void send(const ::protocols::perception::DetectionWrapper& detection_wrapper) = 0;
};

class MessageSender : public IMessageSender {
 public:
  explicit MessageSender(std::unique_ptr<::robocin::IZmqPublisherSocket> detection_socket);

  void send(const ::protocols::perception::Detection& detection) override;
  void send(const ::protocols::perception::DetectionWrapper& detection_wrapper) override;

 private:
  std::unique_ptr<::robocin::IZmqPublisherSocket> detection_socket_;
};

} // namespace perception

#endif // PERCEPTION_MESSAGING_SENDER_MESSAGE_SENDER_H

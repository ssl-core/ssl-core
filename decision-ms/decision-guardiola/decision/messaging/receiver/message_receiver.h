#ifndef DECISION_MESSAGING_RECEIVER_MESSAGE_RECEIVER_H
#define DECISION_MESSAGING_RECEIVER_MESSAGE_RECEIVER_H

#include "decision/messaging/receiver/payload.h"
#include "decision/messaging/receiver/payload_mapper.h"

#include <robocin/network/zmq_datagram.h>
#include <robocin/network/zmq_poller.h>
#include <robocin/network/zmq_subscriber_socket.h>

namespace decision {

class IMessageReceiver {
 public:
  IMessageReceiver() = default;

  IMessageReceiver(const IMessageReceiver&) = delete;
  IMessageReceiver& operator=(const IMessageReceiver&) = delete;
  IMessageReceiver(IMessageReceiver&&) = default;
  IMessageReceiver& operator=(IMessageReceiver&&) = default;

  virtual ~IMessageReceiver() = default;

  virtual Payload receive() = 0;
};

class MessageReceiver : public IMessageReceiver {
 public:
  MessageReceiver(std::unique_ptr<::robocin::IZmqSubscriberSocket> perception_socket,
                  std::unique_ptr<::robocin::IZmqSubscriberSocket> referee_socket,
                  std::unique_ptr<::robocin::IZmqPoller> zmq_poller,
                  std::unique_ptr<IPayloadMapper> payload_mapper);

  [[nodiscard]] Payload receive() override;

 private:
  std::unique_ptr<::robocin::IZmqSubscriberSocket> perception_socket_;
  std::unique_ptr<::robocin::IZmqSubscriberSocket> referee_socket_;
  std::unique_ptr<::robocin::IZmqPoller> zmq_poller_;
  std::unique_ptr<IPayloadMapper> payload_mapper_;
};

} // namespace decision

#endif // DECISION_MESSAGING_RECEIVER_MESSAGE_RECEIVER_H

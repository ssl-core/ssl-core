#ifndef BEHAVIOR_MESSAGING_RECEIVER_MESSAGE_RECEIVER_H
#define BEHAVIOR_MESSAGING_RECEIVER_MESSAGE_RECEIVER_H

#include "behavior/messaging/receiver/payload.h"
#include "behavior/messaging/receiver/payload_mapper.h"

#include <robocin/network/zmq_datagram.h>
#include <robocin/network/zmq_poller.h>
#include <robocin/network/zmq_subscriber_socket.h>

namespace behavior {

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
                  std::unique_ptr<::robocin::IZmqSubscriberSocket> decision_socket,
                  std::unique_ptr<::robocin::IZmqPoller> zmq_poller,
                  std::unique_ptr<IPayloadMapper> payload_mapper);

  [[nodiscard]] Payload receive() override;

 private:
  std::unique_ptr<::robocin::IZmqSubscriberSocket> perception_socket_;
  std::unique_ptr<::robocin::IZmqSubscriberSocket> decision_socket_;
  std::unique_ptr<::robocin::IZmqPoller> zmq_poller_;
  std::unique_ptr<IPayloadMapper> payload_mapper_;
};

} // namespace behavior

#endif // BEHAVIOR_MESSAGING_RECEIVER_MESSAGE_RECEIVER_H

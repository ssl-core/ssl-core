#ifndef COMMUNICATION_MESSAGING_RECEIVER_MESSAGE_RECEIVER_H
#define COMMUNICATION_MESSAGING_RECEIVER_MESSAGE_RECEIVER_H

#include "communication/messaging/receiver/payload.h"
#include "communication/messaging/receiver/payload_mapper.h"

#include <robocin/network/zmq_datagram.h>
#include <robocin/network/zmq_poller.h>
#include <robocin/network/zmq_subscriber_socket.h>

namespace communication {

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
  MessageReceiver(std::unique_ptr<::robocin::IZmqSubscriberSocket> navigation_socket,
                  std::unique_ptr<::robocin::IZmqSubscriberSocket> gateway_socket,
                  std::unique_ptr<::robocin::IZmqPoller> zmq_poller,
                  std::unique_ptr<IPayloadMapper> payload_mapper);

  [[nodiscard]] Payload receive() override;

 private:
  std::unique_ptr<::robocin::IZmqSubscriberSocket> navigation_socket_;
  std::unique_ptr<::robocin::IZmqSubscriberSocket> gateway_socket_;
  std::unique_ptr<::robocin::IZmqPoller> zmq_poller_;
  std::unique_ptr<IPayloadMapper> payload_mapper_;
};

} // namespace communication

#endif // COMMUNICATION_MESSAGING_RECEIVER_MESSAGE_RECEIVER_H
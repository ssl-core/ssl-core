#ifndef COMMUNICATION_MESSAGING_RECEIVER_PAYLOAD_MAPPER_H
#define COMMUNICATION_MESSAGING_RECEIVER_PAYLOAD_MAPPER_H

#include "communication/messaging/receiver/payload.h"

#include <robocin/network/zmq_datagram.h>
#include <span>

namespace communication {

class IPayloadMapper {
 public:
  IPayloadMapper() = default;

  IPayloadMapper(const IPayloadMapper&) = delete;
  IPayloadMapper& operator=(const IPayloadMapper&) = delete;
  IPayloadMapper(IPayloadMapper&&) = default;
  IPayloadMapper& operator=(IPayloadMapper&&) = default;

  virtual ~IPayloadMapper() = default;

  [[nodiscard]] virtual Payload
  fromZmqDatagrams(std::span<const ::robocin::ZmqDatagram> messages) const
      = 0;
};

class PayloadMapper : public IPayloadMapper {
 public:
  PayloadMapper() = default;

  [[nodiscard]] Payload
  fromZmqDatagrams(std::span<const ::robocin::ZmqDatagram> messages) const override;
};

} // namespace communication

#endif // COMMUNICATION_MESSAGING_RECEIVER_PAYLOAD_MAPPER_H

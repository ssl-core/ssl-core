#ifndef REFEREE_MESSAGING_RECEIVER_PAYLOAD_MAPPER_H
#define REFEREE_MESSAGING_RECEIVER_PAYLOAD_MAPPER_H

#include "referee/messaging/receiver/payload.h"

#include <robocin/network/zmq_datagram.h>
#include <span>

namespace referee {

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

} // namespace referee

#endif // REFEREE_MESSAGING_RECEIVER_PAYLOAD_MAPPER_H

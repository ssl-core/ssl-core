#ifndef VISION_NETWORK_SENDER_COMMUNICATION_FACTORY_H
#define VISION_NETWORK_SENDER_COMMUNICATION_FACTORY_H

#include "vision/network/isender_communication.h"

#include <memory>

namespace vision {

enum class SenderCommunicationType {
  zmq,
};

class SenderCommunicationFactory {
 public:
  [[nodiscard]] virtual std::unique_ptr<ISenderCommunication> createZMQCommunication() const = 0;

  virtual ~SenderCommunicationFactory() = default;
};

} // namespace vision

#endif // VISION_NETWORK_SENDER_COMMUNICATION_FACTORY_H

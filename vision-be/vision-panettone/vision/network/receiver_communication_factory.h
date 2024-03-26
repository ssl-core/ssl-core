#ifndef VISION_NETWORK_RECEIVER_COMMUNICATION_FACTORY_H
#define VISION_NETWORK_RECEIVER_COMMUNICATION_FACTORY_H

#include "vision/network/ireceiver_communication.h"

#include <memory>

namespace vision {

enum class ReceiverCommunicationType {
  zmq,
};

class ReceiverCommunicationFactory {
 public:
  [[nodiscard]] virtual std::unique_ptr<IReceiverCommunication> createZMQCommunication() const = 0;

  virtual ~ReceiverCommunicationFactory() = default;
};

} // namespace vision

#endif // VISION_NETWORK_RECEIVER_COMMUNICATION_FACTORY_H

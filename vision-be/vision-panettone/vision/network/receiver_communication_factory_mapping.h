#ifndef VISION_NETWORK_GATEWAY_COMMUNICATION_FACTORY_MAPPING_H
#define VISION_NETWORK_GATEWAY_COMMUNICATION_FACTORY_MAPPING_H

#include "vision/network/ireceiver_communication.h"
#include "vision/network/receiver_communication_factory.h"

#include <functional>
#include <memory>
#include <unordered_map>

namespace vision {

// TODO($ISSUE_N): Update to Singleton pattern.
class ReceiverFactoryMapping {
 public:
  ReceiverFactoryMapping();

  [[nodiscard]] std::unique_ptr<IReceiverCommunication>
  operator[](ReceiverCommunicationType type) const;

 private:
  // TODO($ISSUE_N): Use direct access instead of std::unordered_map.
  std::unordered_map<ReceiverCommunicationType,
                     std::function<std::unique_ptr<IReceiverCommunication>()>>
      factories_;
};

} // namespace vision

#endif // VISION_NETWORK_GATEWAY_COMMUNICATION_FACTORY_MAPPING_H

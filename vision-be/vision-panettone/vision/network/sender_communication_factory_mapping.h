#ifndef VISION_NETWORK_SENDER_COMMUNICATION_FACTORY_MAPPING_H
#define VISION_NETWORK_SENDER_COMMUNICATION_FACTORY_MAPPING_H

#include "vision/network/isender_communication.h"
#include "vision/network/sender_communication_factory.h"

#include <functional>
#include <memory>
#include <unordered_map>

namespace vision {

// TODO($ISSUE_N): Update to Singleton pattern.
class SenderCommunicationFactoryMapping {
 public:
  SenderCommunicationFactoryMapping();

  [[nodiscard]] std::unique_ptr<ISenderCommunication>
  operator[](SenderCommunicationType type) const;

 private:
  // TODO($ISSUE_N): Use direct access instead of std::unordered_map.
  std::unordered_map<SenderCommunicationType,
                     std::function<std::unique_ptr<ISenderCommunication>()>>
      factories_;
};

} // namespace vision

#endif // VISION_NETWORK_SENDER_COMMUNICATION_FACTORY_MAPPING_H

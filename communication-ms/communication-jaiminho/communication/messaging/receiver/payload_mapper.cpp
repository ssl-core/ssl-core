#include "communication/messaging/receiver/payload_mapper.h"

#include "communication/messaging/receiver/payload.h"

#include <robocin/network/zmq_datagram.h>
#include <robocin/output/log.h>
#include <robocin/wip/service_discovery/addresses.h>

namespace communication {
namespace {

namespace service_discovery = robocin::service_discovery;

using ::robocin::wlog;
using ::robocin::ZmqDatagram;
using ::robocin::ilog;

namespace rc {

using ::protocols::navigation::Navigation;

} // namespace rc

namespace tp {

using ::protocols::third_party::game_controller::Referee;

} // namespace tp

} // namespace

Payload PayloadMapper::fromZmqDatagrams(std::span<const ZmqDatagram> messages) const {
  std::vector<rc::Navigation> navigation;
  std::vector<tp::Referee> referee;

  for (const ZmqDatagram& zmq_datagram : messages) {
    if (zmq_datagram.topic() == service_discovery::kNavigationOutputTopic) {
      rc::Navigation navigation_message;
      navigation_message.ParseFromString(std::string{zmq_datagram.message()});
      ilog("Received from navigation: {}", navigation_message.DebugString());
      navigation.emplace_back(std::move(navigation_message));

    } else if (zmq_datagram.topic() == service_discovery::kGameControllerRefereeTopic) {
      tp::Referee referee_message;
      referee_message.ParseFromString(std::string{zmq_datagram.message()});
      referee.emplace_back(std::move(referee_message));

    } else {
      // wlog("zmq_datagram with topic '{}' not processed.", zmq_datagram.topic());
    }
  }

  return Payload{std::move(referee), std::move(navigation)};
}

} // namespace communication

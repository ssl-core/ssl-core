#include "referee/messaging/receiver/payload_mapper.h"

#include "referee/messaging/receiver/payload.h"

#include <robocin/network/zmq_datagram.h>
#include <robocin/output/log.h>
#include <robocin/wip/service_discovery/addresses.h>

namespace referee {
namespace {

namespace service_discovery = robocin::service_discovery;

using ::robocin::wlog;
using ::robocin::ZmqDatagram;

namespace rc {

using ::protocols::perception::Detection;

} // namespace rc

namespace tp {

using ::protocols::third_party::game_controller::Referee;

} // namespace tp

} // namespace

Payload PayloadMapper::fromZmqDatagrams(std::span<const ZmqDatagram> messages) const {
  std::vector<rc::Detection> detections;
  std::vector<tp::Referee> game_controller_referees;

  for (const ZmqDatagram& zmq_datagram : messages) {
    if (zmq_datagram.topic() == service_discovery::kPerceptionDetectionTopic) {
      rc::Detection detection;
      detection.ParseFromString(std::string{zmq_datagram.message()});
      detections.emplace_back(std::move(detection));

    } else if (zmq_datagram.topic() == service_discovery::kGameControllerRefereeTopic) {
      tp::Referee game_controller_referee;
      game_controller_referee.ParseFromString(std::string{zmq_datagram.message()});
      game_controller_referees.emplace_back(std::move(game_controller_referee));
    } else {
      // wlog("zmq_datagram with topic '{}' not processed.", zmq_datagram.topic());
    }
  }

  return Payload{std::move(detections), std::move(game_controller_referees)};
}

} // namespace referee

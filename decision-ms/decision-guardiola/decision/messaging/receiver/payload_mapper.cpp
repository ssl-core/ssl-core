#include "decision/messaging/receiver/payload_mapper.h"

#include "decision/messaging/receiver/payload.h"

#include <protocols/referee/game_status.pb.h>
#include <robocin/network/zmq_datagram.h>
#include <robocin/output/log.h>
#include <robocin/wip/service_discovery/addresses.h>

namespace decision {
namespace {

namespace service_discovery = robocin::service_discovery;

using ::robocin::wlog;
using ::robocin::ZmqDatagram;

namespace rc {

using ::protocols::perception::Detection;
using ::protocols::referee::GameStatus;

} // namespace rc

} // namespace

Payload PayloadMapper::fromZmqDatagrams(std::span<const ZmqDatagram> messages) const {
  std::vector<rc::Detection> detections;
  std::vector<rc::GameStatus> game_statuses;

  for (const ZmqDatagram& zmq_datagram : messages) {
    if (zmq_datagram.topic() == service_discovery::kPerceptionDetectionTopic) {
      rc::Detection detection;
      detection.ParseFromString(std::string{zmq_datagram.message()});
      detections.emplace_back(std::move(detection));

    } else if (zmq_datagram.topic() == service_discovery::kRefereeGameStatusTopic) {
      rc::GameStatus game_status;
      game_status.ParseFromString(std::string{zmq_datagram.message()});
      game_statuses.emplace_back(std::move(game_status));
    } else {
      // wlog("zmq_datagram with topic '{}' not processed.", zmq_datagram.topic());
    }
  }

  return Payload{std::move(detections), std::move(game_statuses)};
}

} // namespace decision

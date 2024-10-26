#include "behavior/messaging/receiver/payload_mapper.h"

#include "behavior/messaging/receiver/payload.h"

#include <robocin/network/zmq_datagram.h>
#include <robocin/output/log.h>
#include <robocin/wip/service_discovery/addresses.h>

namespace behavior {
namespace {

namespace service_discovery = ::robocin::service_discovery;

using ::robocin::ilog;
using ::robocin::ZmqDatagram;

namespace rc {

using ::protocols::decision::Decision;
using ::protocols::perception::Detection;

} // namespace rc

} // namespace

Payload PayloadMapper::fromZmqDatagrams(std::span<const ZmqDatagram> messages) const {
  std::vector<rc::Detection> detections;
  std::vector<rc::Decision> decisions;

  for (const ZmqDatagram& zmq_datagram : messages) {
    if (zmq_datagram.topic() == service_discovery::kPerceptionDetectionTopic) {
      rc::Detection detection;
      detection.ParseFromString(std::string{zmq_datagram.message()});
      detections.emplace_back(std::move(detection));

    } else if (zmq_datagram.topic() == service_discovery::kDecisionTopic) {
      rc::Decision decision;
      decision.ParseFromString(std::string{zmq_datagram.message()});
      decisions.emplace_back(std::move(decision));

    } else {
      // wlog("zmq_datagram with topic '{}' not processed.", zmq_datagram.topic());
    }
  }

  return Payload{std::move(detections), std::move(decisions)};
}

} // namespace behavior

#include "perception/messaging/receiver/payload_mapper.h"

#include "perception/messaging/receiver/payload.h"

#include <robocin/network/zmq_datagram.h>
#include <robocin/output/log.h>
#include <robocin/wip/service_discovery/addresses.h>

namespace perception {
namespace {

namespace service_discovery = ::robocin::service_discovery;

using ::robocin::wlog;
using ::robocin::ZmqDatagram;

namespace tp {

using ::protocols::third_party::detection::SSL_WrapperPacket;
using ::protocols::third_party::game_controller::TrackerWrapperPacket;

} // namespace tp

} // namespace

Payload PayloadMapper::fromZmqDatagrams(std::span<const ZmqDatagram> messages) const {
  std::vector<tp::SSL_WrapperPacket> raw_packets;
  std::vector<tp::TrackerWrapperPacket> tracked_packets;

  for (const ZmqDatagram& zmq_datagram : messages) {
    if (zmq_datagram.topic() == service_discovery::kRawDetectionTopic) {
      tp::SSL_WrapperPacket raw_packet;
      raw_packet.ParseFromString(std::string{zmq_datagram.message()});
      raw_packets.emplace_back(std::move(raw_packet));

    } else if (zmq_datagram.topic() == service_discovery::kTrackedDetectionTopic) {
      tp::TrackerWrapperPacket tracked_packet;
      tracked_packet.ParseFromString(std::string{zmq_datagram.message()});
      tracked_packets.emplace_back(std::move(tracked_packet));
    } else {
      wlog("zmq_datagram with topic '{}' not processed.", zmq_datagram.topic());
    }
  }

  return Payload{std::move(raw_packets), std::move(tracked_packets)};
}

} // namespace perception

#include "perception/messaging/receiver/payload.h"

namespace perception {
namespace {

namespace tp {

using ::protocols::third_party::detection::SSL_WrapperPacket;
using ::protocols::third_party::game_controller::TrackerWrapperPacket;

} // namespace tp

} // namespace

Payload::Payload(std::vector<tp::SSL_WrapperPacket> raw_packets,
                 std::vector<tp::TrackerWrapperPacket> tracked_packets) :
    raw_packets_{std::move(raw_packets)},
    tracked_packets_{std::move(tracked_packets)} {}

std::span<const tp::SSL_WrapperPacket> Payload::getRawPackets() const { return raw_packets_; }

std::span<const tp::TrackerWrapperPacket> Payload::getTrackedPackets() const {
  return tracked_packets_;
}

bool Payload::empty() const { return raw_packets_.empty() and tracked_packets_.empty(); }

} // namespace perception

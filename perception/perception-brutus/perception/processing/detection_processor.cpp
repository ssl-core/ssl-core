#include "perception/processing/detection_processor.h"

#include "perception/processing/raw_detection/mapper/raw_detection_mapper.h"
#include "perception/processing/tracked_detection/mapper/tracked_detection_mapper.h"

#include <protocols/perception/detection.pb.h>
#include <ranges>
#include <tuple>
#include <variant>
#include <vector>

namespace perception {

using protocols::third_party::detection::SSL_GeometryFieldSize;
using Field = protocols::perception::Field;

DetectionWrapper DetectionProcessor::process(const DetectionPacketsPayload& payload) {
  DetectionWrapper wrapper;
  auto [raw_packets, tracked_packets] = splitAndWrapPackets(payload.packets, wrapper);

  Detection detection;
  if (kUseTrackedPacketAsSource and (not tracked_packets.empty())) {
    detection = processTrackedPackets(tracked_packets);
  } else {
    detection = processRawPackets(raw_packets);
  }
  wrapper.mutable_detection()->CopyFrom(detection);

  setFieldSizeFromRawPackets(raw_packets, *wrapper.mutable_detection());

  return wrapper;
}

std::tuple<RawPackets, TrackedPackets>
DetectionProcessor::splitAndWrapPackets(const std::vector<DetectionPacketVariant>& packets,
                                        DetectionWrapper& wrapper) {
  RawPackets raw_packets;
  TrackedPackets tracked_packets;
  for (const auto& packet : packets) {
    if (std::holds_alternative<RawPacket>(packet)) {
      const auto& raw_packet = std::get<RawPacket>(packet);
      raw_packets.push_back(raw_packet);
      wrapper.add_raw_detection()->CopyFrom(raw_packet);
    } else if (std::holds_alternative<TrackedPacket>(packet)) {
      const auto& tracked_packet = std::get<TrackedPacket>(packet);
      tracked_packets.push_back(tracked_packet);
      wrapper.mutable_tracked_detection()->CopyFrom(tracked_packet);
    }
  }
  return {raw_packets, tracked_packets};
}

Detection DetectionProcessor::processRawPackets(const std::vector<RawPacket>& raw_packets) {
  std::vector<RawDetection> raw_detections;
  for (const auto& packet : raw_packets) {
    if (packet.has_detection()) {
      raw_detections.push_back(RawDetectionConverter::convertDetectionFrame(packet.detection()));
    }
  }
  return filter_.process(raw_detections);
}

Detection
DetectionProcessor::processTrackedPackets(const std::vector<TrackedPacket>& tracked_packets) {
  return TrackedDetectionMapper::convertTrackedFrame(tracked_packets.back().tracked_frame());
}

void DetectionProcessor::setFieldSizeFromRawPackets(const std::vector<RawPacket>& raw_packets,
                                                    Detection& detection) {
  for (const auto& raw_packet : std::ranges::reverse_view(raw_packets)) {
    if (raw_packet.has_geometry()) {
      SSL_GeometryFieldSize geometry = raw_packet.geometry().field();

      Field& field = *detection.mutable_field();
      field.set_length(static_cast<float>(geometry.field_length()));
      field.set_width(static_cast<float>(geometry.field_width()));
      field.set_goal_depth(static_cast<float>(geometry.goal_depth()));
      field.set_goal_width(static_cast<float>(geometry.goal_width()));
      field.set_penalty_area_depth(static_cast<float>(geometry.penalty_area_depth()));
      field.set_penalty_area_width(static_cast<float>(geometry.penalty_area_width()));
      field.set_boundary_width(static_cast<float>(geometry.boundary_width()));
      field.set_goal_center_to_penalty_mark(
          static_cast<float>(geometry.goal_center_to_penalty_mark()));
      break;
    }
  }
}

} // namespace perception

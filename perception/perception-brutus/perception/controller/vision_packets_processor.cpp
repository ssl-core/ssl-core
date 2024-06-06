#include "perception/controller/vision_packets_processor.h"

#include "perception/detection_converter/raw_detection_converter.h"

#include <protocols/perception/detection.pb.h>

namespace perception {

using protocols::third_party::detection::SSL_GeometryFieldSize;
using Field = protocols::perception::Field;

// TODO(#ISSUE_N): It may disappear and move logic to filter itself.
Detection VisionPacketsProcessor::process(const std::vector<VisionPackets>& resources) {
  std::vector<RawPacket> raw_packets;
  for (const auto& resource : resources) {
    auto third_party_raw_packets = resource.rawPackets();
    raw_packets.insert(raw_packets.end(),
                       third_party_raw_packets.begin(),
                       third_party_raw_packets.end());
  }

  std::vector<RawDetection> raw_detections;
  for (const auto& packet : raw_packets) {
    if (packet.has_detection()) {
      raw_detections.push_back(RawDetectionConverter::convertDetectionFrame(packet.detection()));
    }
  }

  Detection detection = filter_.process(raw_detections);

  // TODO(#ISSUE_N): Build Field object with latest detection instead of the first one.
  for (const auto& raw_packet : raw_packets) {
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

  return detection;
}

} // namespace perception

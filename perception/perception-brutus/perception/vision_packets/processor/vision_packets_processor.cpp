#include "perception/vision_packets/processor/vision_packets_processor.h"

#include "perception/raw_detection/converter/raw_detection_converter.h"
#include "perception/tracked_frame/converter/tracked_frame_converter.h"

#include <protocols/perception/detection.pb.h>
#include <vector>

namespace perception {

using protocols::third_party::detection::SSL_GeometryFieldSize;
using Field = protocols::perception::Field;

// TODO(#ISSUE_N): It may disappear and move logic to filter itself.
DetectionWrapper VisionPacketsProcessor::process(const std::vector<VisionPackets>& resources) {
  DetectionWrapper wrapper;

  std::vector<RawPacket> flat_raw_packets;
  std::vector<TrackedPacket> flat_tracked_packets;
  for (const auto& resource : resources) {
    auto raw_packets = resource.rawPackets();
    flat_raw_packets.insert(flat_raw_packets.end(), raw_packets.begin(), raw_packets.end());

    auto tracked_packets = resource.trackedPackets();
    flat_tracked_packets.insert(flat_tracked_packets.end(),
                                tracked_packets.begin(),
                                tracked_packets.end());
  }

  if (not flat_raw_packets.empty()) {
    wrapper.mutable_raw_detection();
    for (const auto& raw_packet : flat_raw_packets) {
      wrapper.add_raw_detection()->CopyFrom(raw_packet.detection());
    }
  }
  if (not flat_tracked_packets.empty()) {
    wrapper.mutable_tracked_detection()->CopyFrom(flat_tracked_packets.back());
  }

  Detection detection;
  if (kUseTrackedPacketAsSource and not flat_tracked_packets.empty()) {
    detection = processTrackedPackets(flat_tracked_packets);
  } else {
    detection = processRawPackets(flat_raw_packets);
  }
  wrapper.mutable_detection()->CopyFrom(detection);

  setFieldSizeFromRawPackets(flat_raw_packets, *wrapper.mutable_detection());

  return wrapper;
}

Detection VisionPacketsProcessor::processRawPackets(const std::vector<RawPacket>& raw_packets) {
  std::vector<RawDetection> raw_detections;
  for (const auto& packet : raw_packets) {
    if (packet.has_detection()) {
      raw_detections.push_back(RawDetectionConverter::convertDetectionFrame(packet.detection()));
    }
  }
  return filter_.process(raw_detections);
}

Detection
VisionPacketsProcessor::processTrackedPackets(const std::vector<TrackedPacket>& tracked_packets) {
  return TrackedFrameConverter::convertTrackedFrame(tracked_packets.back().tracked_frame());
}

void VisionPacketsProcessor::setFieldSizeFromRawPackets(const std::vector<RawPacket>& raw_packets,
                                                        Detection& detection) {
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
}

} // namespace perception

#include "perception/processing/detection_processor.h"

#include "perception/messaging/receiver/payload.h"
#include "perception/processing/raw_detection/mappers/raw_detection_mapper.h"

#include <protocols/perception/detection.pb.h>
#include <ranges>

namespace perception {

namespace {

namespace rc {

using ::protocols::perception::Detection;
using ::protocols::perception::DetectionWrapper;
using ::protocols::perception::Field;

} // namespace rc

namespace tp {

using ::protocols::third_party::detection::SSL_GeometryFieldSize;
using ::protocols::third_party::detection::SSL_WrapperPacket;
using ::protocols::third_party::game_controller::TrackerWrapperPacket;

} // namespace tp

// NOLINTBEGIN(*naming*, *magic-numbers*)
// TODO(matheusvtna): retrieve the vision source from perception parameters.
constexpr auto pUseTrackedDetectionAsSource = []() { return true; };
// NOLINTEND(*naming*, *magic-numbers*)

std::vector<tp::SSL_WrapperPacket>
rawWrapperPacketsFromPayloads(std::span<const Payload> payloads) {
  return payloads | std::views::transform(&Payload::getRawPackets) | std::views::join
         | std::ranges::to<std::vector>();
}

std::vector<tp::TrackerWrapperPacket>
trackedPacketsFromPayloads(std::span<const Payload> payloads) {
  return payloads | std::views::transform(&Payload::getTrackedPackets) | std::views::join
         | std::ranges::to<std::vector>();
}

rc::Field processFieldFromRawPackets(std::span<const tp::SSL_WrapperPacket> raw_wrapper_packets) {
  for (const auto& raw_packet : std::ranges::reverse_view(raw_wrapper_packets)) {
    if (raw_packet.has_geometry()) {
      const tp::SSL_GeometryFieldSize& geometry = raw_packet.geometry().field();

      rc::Field field;
      field.set_length(static_cast<float>(geometry.field_length()));
      field.set_width(static_cast<float>(geometry.field_width()));
      field.set_goal_depth(static_cast<float>(geometry.goal_depth()));
      field.set_goal_width(static_cast<float>(geometry.goal_width()));
      field.set_penalty_area_depth(static_cast<float>(geometry.penalty_area_depth()));
      field.set_penalty_area_width(static_cast<float>(geometry.penalty_area_width()));
      field.set_boundary_width(static_cast<float>(geometry.boundary_width()));
      field.set_goal_center_to_penalty_mark(
          static_cast<float>(geometry.goal_center_to_penalty_mark()));

      return field;
    }
  }

  return rc::Field::default_instance();
}

} // namespace

DetectionProcessor::DetectionProcessor(
    std::unique_ptr<IRawDetectionMapper> raw_detection_mapper,
    std::unique_ptr<IRawDetectionFilter> raw_detection_filter,
    std::unique_ptr<ITrackedDetectionFilter> tracked_detection_filter) :
    raw_detection_mapper_{std::move(raw_detection_mapper)},
    raw_detection_filter_{std::move(raw_detection_filter)},
    tracked_detection_filter_{std::move(tracked_detection_filter)} {}

rc::DetectionWrapper DetectionProcessor::process(std::span<const Payload> payloads) {
  rc::DetectionWrapper detection_wrapper;
  rc::Detection& detection = *detection_wrapper.mutable_detection();

  if (pUseTrackedDetectionAsSource()) {
    std::vector<tp::TrackerWrapperPacket> tracked_packets = trackedPacketsFromPayloads(payloads);

    if (rc::Detection processed_detection = tracked_detection_filter_->process(tracked_packets);
        processed_detection.IsInitialized()) {
      detection = std::move(processed_detection);
    }
  } else {
    std::vector<tp::SSL_WrapperPacket> raw_wrapper_packets
        = rawWrapperPacketsFromPayloads(payloads);
    std::vector<RawDetection> raw_detections
        = raw_wrapper_packets
          | std::views::transform(std::bind_front(&IRawDetectionMapper::fromSslWrapperPacket,
                                                  raw_detection_mapper_.get()))
          | std::ranges::to<std::vector>();

    if (rc::Detection processed_detection = raw_detection_filter_->process(raw_detections);
        processed_detection.IsInitialized()) {
      detection = std::move(processed_detection);
    }

    if (rc::Field processed_field = processFieldFromRawPackets(raw_wrapper_packets);
        processed_field.IsInitialized()) {
      last_field_ = std::move(processed_field);
    }
  }

  if (rc::Field& field = *detection.mutable_field(); last_field_.IsInitialized()) {
    field = last_field_;
  }

  // TODO(matheusvtna, joseviccruz): add raw_wrapper_packets and tracked_packets to
  // dectection_wrapper.

  detection.set_serial_id(serial_id_++);
  return detection_wrapper;
}

} // namespace perception

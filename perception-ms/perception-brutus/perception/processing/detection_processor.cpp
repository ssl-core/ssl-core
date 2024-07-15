#include "perception/processing/detection_processor.h"

#include "perception/messaging/receiver/payload.h"
#include "perception/parameters/parameters.h"
#include "perception/processing/raw_detection/mappers/raw_detection_mapper.h"

#include <algorithm>
#include <iterator>
#include <print>
#include <protocols/perception/detection.pb.h>
#include <ranges>
#include <robocin/memory/object_ptr.h>
#include <robocin/output/log.h>

namespace perception {
namespace {

using ::robocin::ilog;
using ::robocin::IPbTimeUtil;
using ::robocin::object_ptr;

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

// TODO(matheusvtna, joseviccruz): use our own framerate generation.
constexpr uint32_t k60Fps = 60;

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

std::vector<RawDetection>
rawDetectionsFromRawPackets(object_ptr<IRawDetectionMapper> raw_detection_mapper,
                            std::span<const tp::SSL_WrapperPacket> raw_wrapper_packets) {
  return raw_wrapper_packets
         | std::views::transform(std::bind_front(&IRawDetectionMapper::fromSslWrapperPacket,
                                                 raw_detection_mapper.get()))
         | std::ranges::to<std::vector>();
}

std::optional<rc::Field>
processFieldFromRawPackets(std::span<const tp::SSL_WrapperPacket> raw_wrapper_packets) {
  auto geometries_view = std::ranges::reverse_view(raw_wrapper_packets)
                         | std::views::filter(&tp::SSL_WrapperPacket::has_geometry)
                         | std::views::take(1);

  if (geometries_view.empty()) {
    return std::nullopt;
  }

  const tp::SSL_GeometryFieldSize& geometry = geometries_view.front().geometry().field();

  rc::Field field;

  if (geometry.has_field_length()) {
    field.set_length(static_cast<float>(geometry.field_length()));
  } else {
    field.set_length(pFieldLength());
  }

  if (geometry.has_field_width()) {
    field.set_width(static_cast<float>(geometry.field_width()));
  } else {
    field.set_width(pFieldWidth());
  }

  if (geometry.has_goal_depth()) {
    field.set_goal_depth(static_cast<float>(geometry.goal_depth()));
  } else {
    field.set_goal_depth(pGoalDepth());
  }

  if (geometry.has_goal_width()) {
    field.set_goal_width(static_cast<float>(geometry.goal_width()));
  } else {
    field.set_goal_width(pGoalWidth());
  }

  if (geometry.has_penalty_area_depth()) {
    field.set_penalty_area_depth(static_cast<float>(geometry.penalty_area_depth()));
  } else {
    field.set_penalty_area_depth(pPenaltyAreaDepth());
  }

  if (geometry.has_penalty_area_width()) {
    field.set_penalty_area_width(static_cast<float>(geometry.penalty_area_width()));
  } else {
    field.set_penalty_area_width(pPenaltyAreaWidth());
  }

  if (geometry.has_boundary_width()) {
    field.set_boundary_width(static_cast<float>(geometry.boundary_width()));
  } else {
    field.set_boundary_width(pBoundaryWidth());
  }

  if (geometry.has_goal_center_to_penalty_mark()) {
    field.set_goal_center_to_penalty_mark(
        static_cast<float>(geometry.goal_center_to_penalty_mark()));
  } else {
    field.set_goal_center_to_penalty_mark(pGoalCenterToPenaltyMark());
  }

  return field;
}

} // namespace

DetectionProcessor::DetectionProcessor(
    std::unique_ptr<IPbTimeUtil> pb_time_util,
    std::unique_ptr<IRawDetectionMapper> raw_detection_mapper,
    std::unique_ptr<IRawDetectionFilter> raw_detection_filter,
    std::unique_ptr<ITrackedDetectionFilter> tracked_detection_filter) :
    pb_time_util_{std::move(pb_time_util)},
    raw_detection_mapper_{std::move(raw_detection_mapper)},
    raw_detection_filter_{std::move(raw_detection_filter)},
    tracked_detection_filter_{std::move(tracked_detection_filter)} {}

std::optional<rc::DetectionWrapper> DetectionProcessor::process(std::span<const Payload> payloads) {
  std::vector<tp::TrackerWrapperPacket> tracked_packets = trackedPacketsFromPayloads(payloads);
  std::vector<tp::SSL_WrapperPacket> raw_wrapper_packets = rawWrapperPacketsFromPayloads(payloads);

  rc::DetectionWrapper detection_wrapper;
  rc::Detection& detection = *detection_wrapper.mutable_detection();

  if (std::optional<rc::Field> processed_field = processFieldFromRawPackets(raw_wrapper_packets)) {
    last_field_ = std::move(processed_field);
  }

  if (pUseTrackedDetectionAsSource()) {
    if (std::optional<rc::Detection> processed_detection
        = tracked_detection_filter_->process(tracked_packets)) {
      detection = std::move(*processed_detection);
    } else {
      // ilog("processed detection from 'tracked_detection_filter' is empty.");

      return std::nullopt;
    }
  } else {
    if (std::optional<rc::Detection> processed_detection = raw_detection_filter_->process(
            rawDetectionsFromRawPackets(raw_detection_mapper_, raw_wrapper_packets))) {
      detection = std::move(*processed_detection);
    } else {
      // ilog("processed detection from 'raw_detection_filter' is empty.");

      return std::nullopt;
    }
  }

  if (last_field_ != std::nullopt) {
    *detection.mutable_field() = *last_field_;
  }

  detection.set_serial_id(++serial_id_);
  *detection.mutable_created_at() = pb_time_util_->getCurrentTime();
  detection.set_framerate(k60Fps);

  detection_wrapper.mutable_tracked_detections()->Add(
      std::make_move_iterator(tracked_packets.begin()),
      std::make_move_iterator(tracked_packets.end()));

  detection_wrapper.mutable_raw_detections()->Add(
      std::make_move_iterator(raw_wrapper_packets.begin()),
      std::make_move_iterator(raw_wrapper_packets.end()));

  return detection_wrapper;
}

} // namespace perception

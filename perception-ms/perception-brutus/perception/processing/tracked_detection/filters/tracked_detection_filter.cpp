#include "perception/processing/tracked_detection/filters/tracked_detection_filter.h"

namespace perception {

namespace {

namespace rc {

using ::protocols::perception::Detection;

} // namespace rc

namespace tp {

using ::protocols::third_party::game_controller::TrackerWrapperPacket;

} // namespace tp

} // namespace

TrackedDetectionFilter::TrackedDetectionFilter(
    std::unique_ptr<ITrackedDetectionMapper> tracked_detection_mapper) :
    tracked_detection_mapper_{std::move(tracked_detection_mapper)} {}

std::optional<rc::Detection>
TrackedDetectionFilter::process(std::span<const tp::TrackerWrapperPacket> tracked_detections) {
  // TODO(matheusvtna): Implement the merge method instead of return the last detection.
  if (!tracked_detections.empty()) {
    return tracked_detection_mapper_->fromTrackedWrapperPacket(tracked_detections.back());
  }

  return std::nullopt;
}

} // namespace perception

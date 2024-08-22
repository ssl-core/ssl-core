#include "perception/processing/raw_detection/filters/raw_detection_filter.h"

#include "perception/processing/raw_detection/filters/camera_filter.h"
#include "perception/processing/raw_detection/filters/camera_filter_factory.h"

#include <protocols/perception/detection.pb.h>
#include <robocin/output/log.h>

namespace perception {
namespace {

namespace rc {

using ::protocols::perception::Detection;

} // namespace rc

using ::robocin::wlog;

// TODO(matheusvtna): Implement the merge method and extrapolate when needed. Maybe move to another
// package "merger" to handle detection, robot and ball merging process separately.
std::optional<rc::Detection> merge(std::span<const rc::Detection> detections) {
  if (detections.empty()) {
    // wlog("no detections to merge.");
    return std::nullopt;
  }

  // TODO(matheusvtna): Implement the merge method instead of return the last detection.
  return detections.back();
}

} // namespace

RawDetectionFilter::RawDetectionFilter(
    std::unique_ptr<ICameraFilter::Factory> camera_filter_factory) :
    camera_filter_factory_{std::move(camera_filter_factory)} {}

std::optional<rc::Detection>
RawDetectionFilter::process(std::span<const RawDetection> raw_detections) {
  // Chunk detections by camera id and process each camera filter separately.
  for (const RawDetection& raw_detection : raw_detections) {
    int camera_id = raw_detection.getCameraId();
    auto it = camera_filters_.find(camera_id);

    if (it == camera_filters_.end()) {
      std::tie(it, std::ignore)
          = camera_filters_.emplace(camera_id, camera_filter_factory_->make());
    }

    it->second->update(raw_detection);
  }

  std::vector<rc::Detection> detections;
  detections.reserve(camera_filters_.size());

  for (const auto& [_, filter] : camera_filters_) {
    if (std::optional<rc::Detection> detection = filter->getDetection()) {
      detections.push_back(std::move(*detection));
    }
  }

  if (std::optional<rc::Detection> detection = merge(detections)) {
    last_detection_ = std::move(*detection);
  }
  return last_detection_;
}

} // namespace perception

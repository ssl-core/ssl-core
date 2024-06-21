#include "perception/processing/raw_detection/filter/vision_filter.h"

namespace perception {

Detection VisionFilter::process(const std::vector<RawDetection>& raw_detections) {
  // Chunk detections by camera ID and process each camera filter separately.
  for (const auto& raw_detection : raw_detections) {
    updateDetectionOnCameraFilter(raw_detection);
  }

  std::vector<Detection> detections;
  detections.reserve(camera_filters_.size());
  for (const auto& [_, filter] : camera_filters_) {
    detections.push_back(filter.getDetection());
  }

  Detection detection = merge(detections);
  detection.set_serial_id(serial_id_++);

  return detection;
}

void VisionFilter::updateDetectionOnCameraFilter(const RawDetection& raw_detection) {
  int camera_id = raw_detection.getCameraID();

  if (camera_filters_.find(camera_id) == camera_filters_.end()) {
    camera_filters_[camera_id] = CameraFilter();
  }

  camera_filters_[camera_id].filter(raw_detection);
}

// TODO(matheusvtna): Implement the merge method and extrapolate when needed. Maybe move to another
// package "merger" to handle detection, robot and ball merging process separately.
Detection VisionFilter::merge(const std::vector<Detection>& detections) {
  if (detections.empty()) {
    std::cout << "[WARNING] No detections to merge." << std::endl;
    return {};
  }

  // TODO(matheusvtna): Implement the merge method instead of return the first detection.
  Detection detection = detections.front();
  detection.set_serial_id(serial_id_++);

  return detection;
}
} // namespace perception

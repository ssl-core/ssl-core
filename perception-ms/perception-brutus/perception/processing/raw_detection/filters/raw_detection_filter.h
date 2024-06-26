#ifndef PERCEPTION_PROCESSING_RAW_DETECTION_FILTER_RAW_DETECTION_FILTER_H
#define PERCEPTION_PROCESSING_RAW_DETECTION_FILTER_RAW_DETECTION_FILTER_H

#include "perception/processing/raw_detection/filters/camera_filter.h"

#include <protocols/perception/detection.pb.h>
#include <unordered_map>

namespace perception {

class IRawDetectionFilter {
 public:
  IRawDetectionFilter() = default;

  IRawDetectionFilter(const IRawDetectionFilter& other) = delete;
  IRawDetectionFilter& operator=(const IRawDetectionFilter& other) = delete;
  IRawDetectionFilter(IRawDetectionFilter&& other) = default;
  IRawDetectionFilter& operator=(IRawDetectionFilter&& other) = default;

  virtual ~IRawDetectionFilter() = default;

  virtual std::optional<::protocols::perception::Detection>
  process(std::span<const RawDetection> raw_detections) = 0;
};

class RawDetectionFilter : public IRawDetectionFilter {
 public:
  explicit RawDetectionFilter(std::unique_ptr<ICameraFilter::Factory> camera_filter_factory);

  std::optional<::protocols::perception::Detection>
  process(std::span<const RawDetection> raw_detections) override;

 private:
  std::optional<::protocols::perception::Detection> last_detection_;
  std::unique_ptr<ICameraFilter::Factory> camera_filter_factory_;
  // TODO(joseviccruz): replace by absl::flat_hash_map
  std::unordered_map<int, std::unique_ptr<ICameraFilter>> camera_filters_;
};

} // namespace perception

#endif // PERCEPTION_PROCESSING_RAW_DETECTION_FILTER_RAW_DETECTION_FILTER_H

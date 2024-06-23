#ifndef PERCEPTION_PROCESSING_TRACKED_DETECTION_FILTERS_TRACKED_DETECTION_FILTER_H
#define PERCEPTION_PROCESSING_TRACKED_DETECTION_FILTERS_TRACKED_DETECTION_FILTER_H

#include "perception/processing/tracked_detection/mappers/tracked_detection_mapper.h"

#include <memory>
#include <protocols/perception/detection.pb.h>

namespace perception {

class ITrackedDetectionFilter {
 public:
  ITrackedDetectionFilter() = default;

  ITrackedDetectionFilter(const ITrackedDetectionFilter& other) = delete;
  ITrackedDetectionFilter& operator=(const ITrackedDetectionFilter& other) = delete;
  ITrackedDetectionFilter(ITrackedDetectionFilter&& other) = default;
  ITrackedDetectionFilter& operator=(ITrackedDetectionFilter&& other) = default;

  virtual ~ITrackedDetectionFilter() = default;

  virtual ::protocols::perception::Detection
  process(std::span<const ::protocols::third_party::game_controller::TrackerWrapperPacket>
              tracked_detections)
      = 0;
};

class TrackedDetectionFilter : public ITrackedDetectionFilter {
 public:
  explicit TrackedDetectionFilter(
      std::unique_ptr<ITrackedDetectionMapper> tracked_detection_mapper);

  ::protocols::perception::Detection
  process(std::span<const ::protocols::third_party::game_controller::TrackerWrapperPacket>
              tracked_detections) override;

 private:
  ::protocols::perception::Detection last_detection_;
  std::unique_ptr<ITrackedDetectionMapper> tracked_detection_mapper_;
};

} // namespace perception

#endif // PERCEPTION_PROCESSING_TRACKED_DETECTION_FILTERS_TRACKED_DETECTION_FILTER_H

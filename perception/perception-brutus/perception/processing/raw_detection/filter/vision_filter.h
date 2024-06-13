#ifndef PERCEPTION_PROCESSING_RAW_DETECTION_FILTER_VISION_FILTER_H
#define PERCEPTION_PROCESSING_RAW_DETECTION_FILTER_VISION_FILTER_H

#include "perception/processing/raw_detection/filter/camera_filter.h"

#include <protocols/perception/detection.pb.h>
#include <unordered_map>
#include <vector>

namespace perception {

using protocols::perception::Detection;

/**
 * @class VisionFilter
 * @brief Applies filters to raw detections and generates merged Detection objects.
 *
 * The VisionFilter class is responsible for applying filters to raw detections,
 * grouping them by camera ID, and merging the filtered detections into
 * a final Detection object.
 */
class VisionFilter {
 public:
  /**
   * @brief Default constructor.
   */
  VisionFilter() = default;

  VisionFilter(const VisionFilter& other) = default;
  VisionFilter& operator=(const VisionFilter& other) = default;
  VisionFilter(VisionFilter&& other) = default;
  VisionFilter& operator=(VisionFilter&& other) = default;

  ~VisionFilter() = default;

  /**
   * @brief Processes the raw detections and generates a merged Detection object.
   * @param raw_detections The vector of raw detections to process.
   * @return The merged Detection object.
   */
  Detection process(const std::vector<RawDetection>& raw_detections);

 private:
  Detection last_detection_; /**< The last generated Detection object. */
  std::unordered_map<int, CameraFilter> camera_filters_; /**< Map of camera ID to VisionFilter. */
  int serial_id_ = 0; /**< Serial ID for the generated Detection object. */

  /**
   * @brief Updates a raw detection on the corresponding camera filter.
   * @param raw_detection The raw detection to append.
   */
  void updateDetectionOnCameraFilter(const RawDetection& raw_detection);

  /**
   * @brief Merges the detections from each camera filter.
   * @param detections The vector of detections to merge.
   * @return The merged Detection object.
   */
  Detection merge(const std::vector<Detection>& detections);
};

} // namespace perception

#endif // PERCEPTION_PROCESSING_RAW_DETECTION_FILTER_VISION_FILTER_H

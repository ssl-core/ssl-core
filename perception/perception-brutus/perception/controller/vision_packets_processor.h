#ifndef PERCEPTION_CONTROLLER_VISION_PACKETS_PROCESSOR_H
#define PERCEPTION_CONTROLLER_VISION_PACKETS_PROCESSOR_H

#include "perception/controller/vision_packets.h"
#include "perception/filter/vision_filter.h"

#include <protocols/perception/detection.pb.h>

namespace perception {

using Detection = protocols::perception::Detection;

/**
 * @class VisionPacketsProcessor
 * @brief Processes vision packets and applies filtering to generate detections.
 *
 * The VisionPacketsProcessor class is responsible for processing a collection of vision packets,
 * applying necessary filtering, and producing a detection result.
 */
class VisionPacketsProcessor {

 public:
  /**
   * @brief Default constructor for VisionPacketsProcessor.
   */
  explicit VisionPacketsProcessor() = default;

  /**
   * @brief Processes a collection of vision packets and generates a detection.
   * @param resources A vector of VisionPackets to be processed.
   * @return A Detection object containing the processed and filtered data.
   *
   * This method takes a vector of VisionPackets, applies the VisionFilter to these packets,
   * and returns a Detection object with the filtered results.
   */
  Detection process(const std::vector<VisionPackets>& resources);

 private:
  VisionFilter filter_; /**< Vision filter used for processing vision packets. */
};

} // namespace perception

#endif // PERCEPTION_CONTROLLER_VISION_PACKETS_PROCESSOR_H

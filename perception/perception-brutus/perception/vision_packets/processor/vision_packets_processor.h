#ifndef PERCEPTION_CONTROLLER_VISION_PACKETS_PROCESSOR_H
#define PERCEPTION_CONTROLLER_VISION_PACKETS_PROCESSOR_H

#include "perception/raw_detection/filter/vision_filter.h"
#include "perception/vision_packets/vision_packets.h"

#include <protocols/perception/detection.pb.h>

namespace perception {

using protocols::perception::Detection;
using protocols::perception::DetectionWrapper;

/**
 * @class VisionPacketsProcessor
 * @brief Processes vision packets and applies filtering to generate detections.
 *
 * The VisionPacketsProcessor class is responsible for processing a collection of vision packets,
 * applying necessary filtering, and producing a detection result.
 */
class VisionPacketsProcessor {

 public:
  explicit VisionPacketsProcessor() = default;
  ~VisionPacketsProcessor() = default;
  VisionPacketsProcessor(const VisionPacketsProcessor&) = delete;
  VisionPacketsProcessor& operator=(const VisionPacketsProcessor&) = delete;
  VisionPacketsProcessor(VisionPacketsProcessor&&) = delete;
  VisionPacketsProcessor& operator=(VisionPacketsProcessor&&) = delete;

  /**
   * @brief Processes a collection of vision packets and generates a detection.
   * @param resources A vector of VisionPackets to be processed.
   * @return A Detection object containing the processed and filtered data.
   *
   * This method takes a vector of VisionPackets, applies the VisionFilter to these packets,
   * and returns a Detection object with the filtered results.
   */
  DetectionWrapper process(const std::vector<VisionPackets>& resources);

 private:
  VisionFilter filter_; /**< Vision filter used for processing vision packets. */

  Detection processRawPackets(const std::vector<RawPacket>& raw_packets);

  static Detection processTrackedPackets(const std::vector<TrackedPacket>& tracked_packets);

  static void setFieldSizeFromRawPackets(const std::vector<RawPacket>& raw_packets,
                                         Detection& detection);

  // TODO(#ISSUE_N): Retrieve the vision source from perception parameters.
  const bool kUseTrackedPacketAsSource = true;
};

} // namespace perception

#endif // PERCEPTION_CONTROLLER_VISION_PACKETS_PROCESSOR_H

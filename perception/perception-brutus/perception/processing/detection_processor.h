#ifndef PERCEPTION_PROCESSING_DETECTION_PROCESSOR_H
#define PERCEPTION_PROCESSING_DETECTION_PROCESSOR_H

#include "perception/processing/raw_detection/filter/vision_filter.h"

#include <protocols/perception/detection.pb.h>
#include <protocols/third_party/detection/raw_wrapper.pb.h>
#include <protocols/third_party/game_controller/tracked_wrapper.pb.h>
#include <variant>
#include <vector>

namespace perception {

using protocols::perception::Detection;
using protocols::perception::DetectionWrapper;

// TODO(matheusvtna): Add TelemetryPacket.
using RawPacket = protocols::third_party::detection::SSL_WrapperPacket;
using TrackedPacket = protocols::third_party::game_controller::TrackerWrapperPacket;
using DetectionPacketVariant = std::variant<RawPacket, TrackedPacket>;

using RawPackets = std::vector<RawPacket>;
using TrackedPackets = std::vector<TrackedPacket>;
using DetectionPackets = std::vector<DetectionPacketVariant>;
struct DetectionPacketsPayload {
  DetectionPackets packets;
};

/**
 * @class PayloadProcessor
 * @brief Processes vision packets and applies filtering to generate detections.
 *
 * The PayloadProcessor class is responsible for processing a collection of vision packets,
 * applying necessary filtering, and producing a detection result.
 */
class DetectionProcessor {

 public:
  explicit DetectionProcessor() = default;
  ~DetectionProcessor() = default;
  DetectionProcessor(const DetectionProcessor&) = delete;
  DetectionProcessor& operator=(const DetectionProcessor&) = delete;
  DetectionProcessor(DetectionProcessor&&) = delete;
  DetectionProcessor& operator=(DetectionProcessor&&) = delete;

  /**
   * @brief Processes a collection of detection packets and generates a detection.
   * @param payloads A vector of DetectionPacketVariant to be processed.
   * @return A Detection object containing the processed and filtered data.
   */
  DetectionWrapper process(const DetectionPacketsPayload& payload);

 private:
  VisionFilter filter_; /**< Vision filter used for processing vision packets. */

  Detection processRawPackets(const std::vector<RawPacket>& raw_packets);

  static Detection processTrackedPackets(const std::vector<TrackedPacket>& tracked_packets);

  // TODO(matheusvtna): Add processTelemetry(...).

  static void setFieldSizeFromRawPackets(const std::vector<RawPacket>& raw_packets,
                                         Detection& detection);

  // TODO(matheusvtna): Retrieve the vision source from perception parameters.
  const bool kUseTrackedPacketAsSource = true;

  /**
   * @brief Splits the packets into raw and tracked packets and wraps them into a DetectionWrapper.
   * @param packets A vector of DetectionPacketVariant to be split and wrapped.
   * @param wrapper A reference to the DetectionWrapper to be filled with the packets.
   * @return A tuple containing the raw and tracked packets.
   */
  static std::tuple<RawPackets, TrackedPackets>
  splitAndWrapPackets(const std::vector<DetectionPacketVariant>& packets,
                      DetectionWrapper& wrapper);
};

} // namespace perception

#endif // PERCEPTION_PROCESSING_DETECTION_PROCESSOR_H

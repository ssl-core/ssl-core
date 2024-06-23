#ifndef PERCEPTION_PROCESSING_TRACKED_DETECTION_MAPPER_TRACKED_DETECTION_MAPPER_H
#define PERCEPTION_PROCESSING_TRACKED_DETECTION_MAPPER_TRACKED_DETECTION_MAPPER_H

#include <protocols/perception/detection.pb.h>
#include <protocols/third_party/game_controller/tracked.pb.h>
#include <protocols/third_party/game_controller/tracked_wrapper.pb.h>

namespace perception {

class ITrackedDetectionMapper {
 public:
  ITrackedDetectionMapper() = default;

  ITrackedDetectionMapper(const ITrackedDetectionMapper&) = delete;
  ITrackedDetectionMapper& operator=(const ITrackedDetectionMapper&) = delete;
  ITrackedDetectionMapper(ITrackedDetectionMapper&&) = default;
  ITrackedDetectionMapper& operator=(ITrackedDetectionMapper&&) = default;

  virtual ~ITrackedDetectionMapper() = default;

  virtual ::protocols::perception::Detection fromTrackedWrapperPacket(
      const ::protocols::third_party::game_controller::TrackerWrapperPacket& tracked_wrapper_packet)
      = 0;
};

/**
 * @class TrackedDetectionMapper
 * @brief Converts TrackedFrame data to Detection format.
 *
 * The TrackedDetectionMapper class provides static methods to convert
 * tracked frames and their components (robots and balls) from the
 * TrackedFrame format to the Detection format.
 */
class TrackedDetectionMapper : public ITrackedDetectionMapper {
 public:
  TrackedDetectionMapper() = default;

  /**
   * @brief Converts an TrackedFrame to a Detection object.
   * @param tracked_frame The TrackedFrame to be converted.
   * @return A Detection object containing the converted data.
   *
   * This method takes an TrackedFrame and converts it into a Detection
   * object, which includes the processed robot and ball data.
   */
  ::protocols::perception::Detection fromTrackedWrapperPacket(
      const ::protocols::third_party::game_controller::TrackerWrapperPacket& tracked_wrapper_packet)
      override;
};

} // namespace perception

#endif // PERCEPTION_PROCESSING_TRACKED_DETECTION_MAPPER_TRACKED_DETECTION_MAPPER_H

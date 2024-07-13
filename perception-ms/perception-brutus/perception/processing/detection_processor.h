#ifndef PERCEPTION_PROCESSING_DETECTION_PROCESSOR_H
#define PERCEPTION_PROCESSING_DETECTION_PROCESSOR_H

// TODO(joseviccruz, matheusvtna): maybe map payload before.
#include "perception/messaging/receiver/payload.h"
#include "perception/processing/raw_detection/filters/raw_detection_filter.h"
#include "perception/processing/raw_detection/mappers/raw_detection_mapper.h"
#include "perception/processing/tracked_detection/filters/tracked_detection_filter.h"

#include <google/protobuf/timestamp.pb.h>
#include <google/protobuf/util/time_util.h>
#include <protocols/perception/detection.pb.h>
#include <protocols/third_party/detection/raw_wrapper.pb.h>
#include <protocols/third_party/game_controller/tracked_wrapper.pb.h>
#include <robocin/third_party/adaptors/pb_time_util.h>

namespace perception {

class IDetectionProcessor {
 public:
  IDetectionProcessor() = default;

  IDetectionProcessor(const IDetectionProcessor&) = delete;
  IDetectionProcessor& operator=(const IDetectionProcessor&) = delete;
  IDetectionProcessor(IDetectionProcessor&&) = default;
  IDetectionProcessor& operator=(IDetectionProcessor&&) = default;

  virtual ~IDetectionProcessor() = default;

  virtual std::optional<::protocols::perception::DetectionWrapper>
  process(std::span<const Payload> payloads) = 0;
};

class DetectionProcessor : public IDetectionProcessor {
 public:
  DetectionProcessor(std::unique_ptr<::robocin::IPbTimeUtil> pb_time_util,
                     std::unique_ptr<IRawDetectionMapper> raw_detection_mapper,
                     std::unique_ptr<IRawDetectionFilter> raw_detection_filter,
                     std::unique_ptr<ITrackedDetectionFilter> tracked_detection_filter);

  std::optional<::protocols::perception::DetectionWrapper>
  process(std::span<const Payload> payloads) override;

 private:
  // TODO(matheusvtna, joseviccruz): discuss since serial_id must be the same when using raw and
  // tracked.
  uint64_t serial_id_{0};
  std::unique_ptr<::robocin::IPbTimeUtil> pb_time_util_;
  std::optional<::protocols::perception::Field> last_field_;
  std::unique_ptr<IRawDetectionMapper> raw_detection_mapper_;
  std::unique_ptr<IRawDetectionFilter> raw_detection_filter_;
  std::unique_ptr<ITrackedDetectionFilter> tracked_detection_filter_;
};

} // namespace perception

#endif // PERCEPTION_PROCESSING_DETECTION_PROCESSOR_H

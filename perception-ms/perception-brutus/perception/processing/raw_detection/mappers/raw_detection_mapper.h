#ifndef PERCEPTION_PROCESSING_RAW_DETECTION_MAPPER_RAW_DETECTION_MAPPER_H
#define PERCEPTION_PROCESSING_RAW_DETECTION_MAPPER_RAW_DETECTION_MAPPER_H

#include "perception/processing/raw_detection/entities/raw_detection.h"

#include <protocols/third_party/detection/raw_wrapper.pb.h>

namespace perception {

class IRawDetectionMapper {
 public:
  IRawDetectionMapper() = default;

  IRawDetectionMapper(const IRawDetectionMapper&) = delete;
  IRawDetectionMapper& operator=(const IRawDetectionMapper&) = delete;
  IRawDetectionMapper(IRawDetectionMapper&&) = delete;
  IRawDetectionMapper& operator=(IRawDetectionMapper&&) = delete;

  virtual ~IRawDetectionMapper() = default;

  [[nodiscard]] virtual RawDetection fromSslWrapperPacket(
      const ::protocols::third_party::detection::SSL_WrapperPacket& wrapper_packet) const
      = 0;
};

class RawDetectionMapper : public IRawDetectionMapper {
 public:
  RawDetectionMapper() = default;

  [[nodiscard]] RawDetection fromSslWrapperPacket(
      const ::protocols::third_party::detection::SSL_WrapperPacket& wrapper_packet) const override;
};

} // namespace perception

#endif // PERCEPTION_PROCESSING_RAW_DETECTION_MAPPER_RAW_DETECTION_MAPPER_H

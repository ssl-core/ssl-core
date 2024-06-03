#ifndef PERCEPTION_CONVERTER_RAW_DETECTION_CONVERTER_H
#define PERCEPTION_CONVERTER_RAW_DETECTION_CONVERTER_H

#include "perception/detection_converter/raw_detection.h"
#include "protocols/third_party/detection/raw.pb.h"

namespace perception {

using protocols::third_party::detection::SSL_DetectionBall;
using protocols::third_party::detection::SSL_DetectionFrame;
using protocols::third_party::detection::SSL_DetectionRobot;

/**
 * @class RawDetectionConverter
 * @brief Converts SSL_DetectionFrame data to RawDetection format.
 *
 * The RawDetectionConverter class provides static methods to convert
 * detection frames and their components (robots and balls) from the
 * SSL_DetectionFrame format to the RawDetection format.
 */
class RawDetectionConverter {
 public:
  /**
   * @brief Converts an SSL_DetectionFrame to a RawDetection object.
   * @param detection_frame The SSL_DetectionFrame to be converted.
   * @return A RawDetection object containing the converted data.
   *
   * This method takes an SSL_DetectionFrame and converts it into a
   * RawDetection object, which includes the processed robot and ball data.
   */
  static RawDetection convertDetectionFrame(const SSL_DetectionFrame& detection_frame);

 private:
  /**
   * @brief Converts an SSL_DetectionRobot to a RawRobot object.
   * @param robot The SSL_DetectionRobot to be converted.
   * @param color The color of the robot.
   * @return A RawRobot object containing the converted data.
   *
   * This method takes an SSL_DetectionRobot and converts it into a
   * RawRobot object, incorporating the specified robot color.
   */
  static RawRobot convertRobot(const SSL_DetectionRobot& robot, RobotColor color);

  /**
   * @brief Converts an SSL_DetectionBall to a RawBall object.
   * @param ball The SSL_DetectionBall to be converted.
   * @return A RawBall object containing the converted data.
   */
  static RawBall convertBall(const SSL_DetectionBall& ball);
};

} // namespace perception

#endif // PERCEPTION_CONVERTER_RAW_DETECTION_CONVERTER_H

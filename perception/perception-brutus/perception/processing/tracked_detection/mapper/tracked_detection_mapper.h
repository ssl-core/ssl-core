#ifndef PERCEPTION_PROCESSING_TRACKED_DETECTION_MAPPER_TRACKED_DETECTION_MAPPER_H
#define PERCEPTION_PROCESSING_TRACKED_DETECTION_MAPPER_TRACKED_DETECTION_MAPPER_H

#include <gtest/gtest_prod.h>
#include <protocols/perception/detection.pb.h>
#include <protocols/third_party/game_controller/tracked.pb.h>

namespace perception {

using protocols::perception::Ball;
using protocols::perception::Detection;
using protocols::perception::Robot;

using protocols::third_party::game_controller::TrackedBall;
using protocols::third_party::game_controller::TrackedFrame;
using protocols::third_party::game_controller::TrackedRobot;

/**
 * @class TrackedDetectionMapper
 * @brief Converts TrackedFrame data to Detection format.
 *
 * The TrackedDetectionMapper class provides static methods to convert
 * tracked frames and their components (robots and balls) from the
 * TrackedFrame format to the Detection format.
 */
class TrackedDetectionMapper {
 public:
  /**
   * @brief Converts an TrackedFrame to a Detection object.
   * @param tracked_frame The TrackedFrame to be converted.
   * @return A Detection object containing the converted data.
   *
   * This method takes an TrackedFrame and converts it into a Detection
   * object, which includes the processed robot and ball data.
   */
  static Detection convertTrackedFrame(const TrackedFrame& tracked_frame);

 private:
  /**
   * @brief Converts an TrackedRobot to a Robot object.
   * @param tracked_robot The TrackedRobot to be converted.
   * @return A Robot object containing the converted data.
   */
  static Robot convertTrackedRobot(const TrackedRobot& tracked_robot);

  /**
   * @brief Converts an TrackedBall to a Ball object.
   * @param tracked_ball The TrackedBall to be converted.
   * @return A Ball object containing the converted data.
   */
  static Ball convertTrackedBall(const TrackedBall& ball);

  FRIEND_TEST(TrackedDetectionMapperTest, GivenTrackedBallWhenConvertTrackedBallReturnsBall);
  FRIEND_TEST(TrackedDetectionMapperTest, GivenTrackedRobotWhenConvertTrackedRobotReturnsRobot);
};

} // namespace perception

#endif // PERCEPTION_PROCESSING_TRACKED_DETECTION_MAPPER_TRACKED_DETECTION_MAPPER_H

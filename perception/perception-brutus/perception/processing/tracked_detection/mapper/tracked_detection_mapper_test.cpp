#include "perception/processing/tracked_detection/mapper/tracked_detection_mapper.h"
#include "protocols/third_party/game_controller/tracked.pb.h"

#include <gmock/gmock.h>
#include <google/protobuf/text_format.h>
#include <google/protobuf/util/message_differencer.h>
#include <gtest/gtest.h>
#include <protocols/third_party/game_controller/common.pb.h>

// TODO(joseviccruz): Add mockable clock to common
// TODO(joseviccruz): Add protobuf test utilities to common
// TODO(joseviccruz): Add cpp guidelines for unit testing

namespace perception {

using ::google::protobuf::TextFormat;
using ::google::protobuf::util::MessageDifferencer;
namespace input {

constexpr const char* kFlyingBallProto = R"pb(
pos {
  x: 4321
  y: 1234
  z: 200
}
vel {
  x: 200
  y: 123
  z: 100
}
visibility: 0.4
)pb";

constexpr const char* kFlatBallProto = R"pb(
pos {
  x: 321
  y: -123
  z: 1.5
}
vel {
  x: -350
  y: 100
  z: 0.5
}
visibility: 0.8
)pb";

constexpr const char* kBlueRobotProto = R"pb(
robot_id {
  id: 1
  team: BLUE
}
pos {
  x: 1000
  y: -500
}
orientation: -1.07
vel {
  x: 500
  y: -1000
}
vel_angular: 0.5
visibility: 0.9
)pb";

constexpr const char* kYellowRobotProto = R"pb(
robot_id {
  id: 1
  team: YELLOW
}
pos {
  x: 1000
  y: -500
}
orientation: -1.07
vel {
  x: 500
  y: -1000
}
vel_angular: 0.5
visibility: 0.9
)pb";

constexpr const char* kUnknownRobotProto = R"pb(
robot_id {
  id: 1
  team: UNKNOWN
}
pos {
  x: 1000
  y: -500
}
orientation: -1.07
vel {
  x: 500
  y: -1000
}
vel_angular: 0.5
visibility: 0.9
)pb";

} // namespace input

namespace expected {

constexpr const char* kRobotBlueProto = R"pb(
confidence: 0.9
robot_id {
  color: COLOR_BLUE
  number: 1
}
position {
  x: 1000
  y: -500
}
angle: -1.07
velocity {
  x: 500
  y: -1000
}
angular_velocity: 0.5
)pb";

constexpr const char* kRobotYellowProto = R"pb(
confidence: 0.9
robot_id {
  color: COLOR_YELLOW
  number: 1
}
position {
  x: 1000
  y: -500
}
angle: -1.07
velocity {
  x: 500
  y: -1000
}
angular_velocity: 0.5
)pb";

constexpr const char* kRobotUnspecifiedProto = R"pb(
confidence: 0.9
robot_id {
  color: COLOR_UNSPECIFIED
  number: 1
}
position {
  x: 1000
  y: -500
}
angle: -1.07
velocity {
  x: 500
  y: -1000
}
angular_velocity: 0.5
)pb";

constexpr const char* kFlyingBallProto = R"pb(
confidence: 0.4
position {
  x: 4321
  y: 1234
  z: 200
}
velocity {
  x: 200
  y: 123
  z: 100
}
)pb";

constexpr const char* kFlatBallProto = R"pb(
confidence: 0.8
position {
  x: 321
  y: -123
  z: 1.5
}
velocity {
  x: -350
  y: 100
  z: 0.5
}
)pb";

} // namespace expected

class TrackedFrameMock {
 public:
  static protocols::third_party::game_controller::TrackedBall createFlyingTrackedBall() {
    protocols::third_party::game_controller::TrackedBall tracked_ball;
    TextFormat::ParseFromString(input::kFlyingBallProto, &tracked_ball);
    return tracked_ball;
  }

  static protocols::third_party::game_controller::TrackedBall createTrackedFlatBall() {
    protocols::third_party::game_controller::TrackedBall tracked_ball;
    TextFormat::ParseFromString(input::kFlatBallProto, &tracked_ball);
    return tracked_ball;
  }

  static protocols::third_party::game_controller::TrackedRobot
  createTrackedRobot(const protocols::third_party::game_controller::Team& team) {
    protocols::third_party::game_controller::TrackedRobot tracked_robot;
    switch (team) {
      case protocols::third_party::game_controller::Team::BLUE:
        TextFormat::ParseFromString(input::kBlueRobotProto, &tracked_robot);
        break;
      case protocols::third_party::game_controller::Team::YELLOW:
        TextFormat::ParseFromString(input::kYellowRobotProto, &tracked_robot);
        break;
      case protocols::third_party::game_controller::Team::UNKNOWN:
        TextFormat::ParseFromString(input::kUnknownRobotProto, &tracked_robot);
    }

    return tracked_robot;
  }

  static protocols::third_party::game_controller::TrackedFrame createTrackedFrame() {
    protocols::third_party::game_controller::TrackedFrame tracked_frame;
    // NOLINTBEGIN(readability-magic-numbers)
    tracked_frame.set_frame_number(42);
    tracked_frame.set_timestamp(1234567890.0);
    // NOLINTEND(readability-magic-numbers)

    *tracked_frame.add_balls() = createFlyingTrackedBall();
    *tracked_frame.add_balls() = createTrackedFlatBall();

    *tracked_frame.add_robots()
        = createTrackedRobot(protocols::third_party::game_controller::Team::BLUE);
    *tracked_frame.add_robots()
        = createTrackedRobot(protocols::third_party::game_controller::Team::YELLOW);
    *tracked_frame.add_robots()
        = createTrackedRobot(protocols::third_party::game_controller::Team::UNKNOWN);

    return tracked_frame;
  }
};

TEST(TrackedDetectionMapperTest, GivenTrackedBallWhenConvertTrackedBallReturnsBall) {
  TrackedBall tracked_ball = TrackedFrameMock::createFlyingTrackedBall();

  Ball ball = TrackedDetectionMapper::convertTrackedBall(tracked_ball);

  Ball expected;
  TextFormat::ParseFromString(expected::kFlyingBallProto, &expected);
  ASSERT_TRUE(MessageDifferencer::Equals(ball, expected));
}

TEST(TrackedDetectionMapperTest, GivenTrackedRobotWhenConvertTrackedRobotReturnsRobot) {
  TrackedRobot tracked_robot
      = TrackedFrameMock::createTrackedRobot(protocols::third_party::game_controller::Team::BLUE);
  Robot robot = TrackedDetectionMapper::convertTrackedRobot(tracked_robot);

  Robot expected;
  TextFormat::ParseFromString(expected::kRobotBlueProto, &expected);

  ASSERT_TRUE(MessageDifferencer::Equals(robot, expected));
}

TEST(TrackedDetectionMapperTest, GivenTrackedFrameWhenConvertTrackedFrameReturnsDetection) {
  TrackedFrame tracked_frame = TrackedFrameMock::createTrackedFrame();
  Detection detection = TrackedDetectionMapper::convertTrackedFrame(tracked_frame);

  // Ignore the `created_at` attribute match for the sake of this test since the
  // TrackedDetectionMapper is not responsible for converting that.
  ASSERT_EQ(detection.serial_id(), 42);
  ASSERT_EQ(detection.balls_size(), 2);
  ASSERT_EQ(detection.robots_size(), 3);

  const Ball& flying_ball = detection.balls(0);
  Ball expected_flying_ball;
  TextFormat::ParseFromString(expected::kFlyingBallProto, &expected_flying_ball);
  ASSERT_TRUE(MessageDifferencer::Equals(flying_ball, expected_flying_ball));

  const Ball& flat_ball = detection.balls(1);
  Ball expected_flat_ball;
  TextFormat::ParseFromString(expected::kFlatBallProto, &expected_flat_ball);
  ASSERT_TRUE(MessageDifferencer::Equals(flat_ball, expected_flat_ball));

  const Robot& robot_blue = detection.robots(0);
  Robot expected_robot_blue;
  TextFormat::ParseFromString(expected::kRobotBlueProto, &expected_robot_blue);
  ASSERT_TRUE(MessageDifferencer::Equals(robot_blue, expected_robot_blue));

  const Robot& robot_yellow = detection.robots(1);
  Robot expected_robot_yellow;
  TextFormat::ParseFromString(expected::kRobotYellowProto, &expected_robot_yellow);
  ASSERT_TRUE(MessageDifferencer::Equals(robot_yellow, expected_robot_yellow));

  const Robot& robot_unspecified = detection.robots(2);
  Robot expected_robot_unspecified;
  TextFormat::ParseFromString(expected::kRobotUnspecifiedProto, &expected_robot_unspecified);
  ASSERT_TRUE(MessageDifferencer::Equals(robot_unspecified, expected_robot_unspecified));
}

} // namespace perception

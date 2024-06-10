#include "perception/raw_detection/entities/robot_id.h"
#include "perception/tracked_frame/converter/tracked_frame_converter.h"
#include "protocols/third_party/game_controller/tracked.pb.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <protocols/third_party/game_controller/common.pb.h>

namespace perception {

using ::testing::Eq;

class TrackedFrameMock {
 public:
  static protocols::third_party::game_controller::TrackedBall createTrackedBall() {
    protocols::third_party::game_controller::TrackedBall tracked_ball;
    // NOLINTBEGIN(readability-magic-numbers)
    tracked_ball.mutable_pos()->set_x(4321.0F);
    tracked_ball.mutable_pos()->set_y(1234.0F);
    tracked_ball.mutable_pos()->set_z(200.0F);
    tracked_ball.mutable_vel()->set_x(200.0F);
    tracked_ball.mutable_vel()->set_y(123.0F);
    tracked_ball.mutable_vel()->set_z(100.0F);
    tracked_ball.set_visibility(0.8F);
    // NOLINTEND(readability-magic-numbers)
    return tracked_ball;
  }

  static protocols::third_party::game_controller::TrackedBall createTrackedFlatBall() {
    protocols::third_party::game_controller::TrackedBall tracked_ball;
    // NOLINTBEGIN(readability-magic-numbers)
    tracked_ball.mutable_pos()->set_x(321.0F);
    tracked_ball.mutable_pos()->set_y(123.0F);
    tracked_ball.mutable_pos()->set_z(1.5F);
    tracked_ball.mutable_vel()->set_x(350.0F);
    tracked_ball.mutable_vel()->set_y(100.0F);
    tracked_ball.mutable_vel()->set_z(0.5F);
    tracked_ball.set_visibility(0.8F);
    // NOLINTEND(readability-magic-numbers)
    return tracked_ball;
  }

  static protocols::third_party::game_controller::TrackedRobot
  createTrackedRobot(const protocols::third_party::game_controller::Team& team) {
    protocols::third_party::game_controller::TrackedRobot tracked_robot;
    // NOLINTBEGIN(readability-magic-numbers)
    tracked_robot.mutable_robot_id()->set_id(1);
    tracked_robot.mutable_robot_id()->set_team(team);
    tracked_robot.mutable_pos()->set_x(1.0F);
    tracked_robot.mutable_pos()->set_y(2.0F);
    tracked_robot.set_orientation(1.57F);
    tracked_robot.mutable_vel()->set_x(0.1F);
    tracked_robot.mutable_vel()->set_y(0.2F);
    tracked_robot.set_vel_angular(0.05F);
    tracked_robot.set_visibility(0.9F);
    // NOLINTEND(readability-magic-numbers)
    return tracked_robot;
  }

  static protocols::third_party::game_controller::TrackedFrame createTrackedFrame() {
    protocols::third_party::game_controller::TrackedFrame tracked_frame;
    // NOLINTBEGIN(readability-magic-numbers)
    tracked_frame.set_frame_number(42);
    tracked_frame.set_timestamp(1234567890.0);
    // NOLINTEND(readability-magic-numbers)

    *tracked_frame.add_balls() = createTrackedBall();
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

TEST(TrackedFrameConverterTest, GivenTrackedBallWhenConvertTrackedBallReturnsBall) {
  TrackedBall tracked_ball = TrackedFrameMock::createTrackedBall();

  Ball ball = TrackedFrameConverter::convertTrackedBall(tracked_ball);

  ASSERT_FLOAT_EQ(ball.confidence(), 0.8F);
  ASSERT_EQ(ball.position().x(), 4321.0F);
  ASSERT_EQ(ball.position().y(), 1234.0F);
  ASSERT_EQ(ball.position().z(), 200.0F);
  ASSERT_EQ(ball.velocity().x(), 200.0F);
  ASSERT_EQ(ball.velocity().y(), 123.0F);
  ASSERT_EQ(ball.velocity().z(), 100.0F);
}

TEST(TrackedFrameConverterTest, GivenTrackedRobotWhenConvertTrackedRobotReturnsRobot) {
  TrackedRobot tracked_robot
      = TrackedFrameMock::createTrackedRobot(protocols::third_party::game_controller::Team::BLUE);
  Robot robot = TrackedFrameConverter::convertTrackedRobot(tracked_robot);

  ASSERT_FLOAT_EQ(robot.confidence(), 0.9F);
  ASSERT_EQ(robot.robot_id().number(), 1);
  ASSERT_EQ(robot.robot_id().color(), eColorBlue);
  ASSERT_EQ(robot.position().x(), 1.0F);
  ASSERT_EQ(robot.position().y(), 2.0F);
  ASSERT_FLOAT_EQ(robot.angle(), 1.57F);
  ASSERT_EQ(robot.velocity().x(), 0.1F);
  ASSERT_EQ(robot.velocity().y(), 0.2F);
  ASSERT_FLOAT_EQ(robot.angular_velocity(), 0.05F);
}

TEST(TrackedFrameConverterTest, GivenTrackedFrameWhenConvertTrackedFrameReturnsDetection) {
  TrackedFrame tracked_frame = TrackedFrameMock::createTrackedFrame();
  Detection detection = TrackedFrameConverter::convertTrackedFrame(tracked_frame);

  ASSERT_EQ(detection.serial_id(), 42);
  ASSERT_EQ(detection.created_at().seconds(), 1234567890);
  ASSERT_EQ(detection.balls_size(), 2);
  ASSERT_EQ(detection.robots_size(), 3);

  const Ball& ball = detection.balls(0);
  ASSERT_FLOAT_EQ(ball.confidence(), 0.8F);
  ASSERT_EQ(ball.position().x(), 4321.0F);
  ASSERT_EQ(ball.position().y(), 1234.0F);
  ASSERT_EQ(ball.position().z(), 200.0F);
  ASSERT_EQ(ball.velocity().x(), 200.0F);
  ASSERT_EQ(ball.velocity().y(), 123.0F);
  ASSERT_EQ(ball.velocity().z(), 100.0F);

  const Robot& robot_blue = detection.robots(0);
  ASSERT_FLOAT_EQ(robot_blue.confidence(), 0.9F);
  ASSERT_EQ(robot_blue.robot_id().number(), 1);
  ASSERT_EQ(robot_blue.robot_id().color(), eColorBlue);
  ASSERT_EQ(robot_blue.position().x(), 1.0F);
  ASSERT_EQ(robot_blue.position().y(), 2.0F);
  ASSERT_FLOAT_EQ(robot_blue.angle(), 1.57F);
  ASSERT_EQ(robot_blue.velocity().x(), 0.1F);
  ASSERT_EQ(robot_blue.velocity().y(), 0.2F);
  ASSERT_FLOAT_EQ(robot_blue.angular_velocity(), 0.05F);

  const Robot& robot_yellow = detection.robots(1);
  ASSERT_FLOAT_EQ(robot_yellow.confidence(), 0.9F);
  ASSERT_EQ(robot_yellow.robot_id().number(), 1);
  ASSERT_EQ(robot_yellow.robot_id().color(), eColorYellow);
  ASSERT_EQ(robot_yellow.position().x(), 1.0F);
  ASSERT_EQ(robot_yellow.position().y(), 2.0F);
  ASSERT_FLOAT_EQ(robot_yellow.angle(), 1.57F);
  ASSERT_EQ(robot_yellow.velocity().x(), 0.1F);
  ASSERT_EQ(robot_yellow.velocity().y(), 0.2F);
  ASSERT_FLOAT_EQ(robot_yellow.angular_velocity(), 0.05F);

  const Robot& robot_unspecified = detection.robots(2);
  ASSERT_FLOAT_EQ(robot_unspecified.confidence(), 0.9F);
  ASSERT_EQ(robot_unspecified.robot_id().number(), 1);
  ASSERT_EQ(robot_unspecified.robot_id().color(), eColorUnspecified);
  ASSERT_EQ(robot_unspecified.position().x(), 1.0F);
  ASSERT_EQ(robot_unspecified.position().y(), 2.0F);
  ASSERT_FLOAT_EQ(robot_unspecified.angle(), 1.57F);
  ASSERT_EQ(robot_unspecified.velocity().x(), 0.1F);
  ASSERT_EQ(robot_unspecified.velocity().y(), 0.2F);
  ASSERT_FLOAT_EQ(robot_unspecified.angular_velocity(), 0.05F);
}

} // namespace perception

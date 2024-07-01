#include "perception/processing/tracked_detection/mappers/tracked_detection_mapper.h"

#include <array>
#include <gmock/gmock.h>
#include <google/protobuf/text_format.h>
#include <google/protobuf/timestamp.pb.h>
#include <google/protobuf/util/message_differencer.h>
#include <google/protobuf/util/time_util.h>
#include <gtest/gtest.h>
#include <print>
#include <protocols/third_party/game_controller/common.pb.h>
#include <robocin/parameters/parameters.h>
#include <robocin/third_party/adaptors/pb_time_util.h>
#include <robocin/third_party/google/protobuf/test_textproto.h>

// TODO(joseviccruz): Add cpp guidelines for unit testing

namespace perception {

namespace parameters = ::robocin::parameters;

using ::google::protobuf::EqualsProto;
using ::google::protobuf::ParseTextOrDie;
using ::google::protobuf::Timestamp;
using ::google::protobuf::util::TimeUtil;
using ::robocin::IPbTimeUtil;
using ::testing::Test;

namespace rc {

using ::protocols::perception::Detection;

} // namespace rc

namespace tp {

using ::protocols::third_party::game_controller::TrackerWrapperPacket;

} // namespace tp

constexpr uint64_t k10Seconds = 10;

class TrackedDetectionMapperTest : public Test {
 protected:
  void SetUp() override {
    parameters::HandlerEngine{}.update(std::to_array({
        parameters::Value{3, "robot_radius", 90.0F},   // NOLINT(*numbers*)
        parameters::Value{4, "robot_height", 140.0F},  // NOLINT(*numbers*)
        parameters::Value{5, "dribbler_width", 60.0F}, // NOLINT(*numbers*)
    }));
  }

  static std::unique_ptr<TrackedDetectionMapper> makeMapper(Timestamp now) {
    return std::make_unique<TrackedDetectionMapper>(
        std::make_unique<MockPbTimeUtil>(std::move(now)));
  }

 private:
  class MockPbTimeUtil : public IPbTimeUtil {
   public:
    explicit MockPbTimeUtil(Timestamp timestamp) : timestamp_{std::move(timestamp)} {}

    Timestamp getCurrentTime() override { return timestamp_; }

   private:
    Timestamp timestamp_;
  };
};

TEST_F(TrackedDetectionMapperTest, WhenMapperReceivesSingleBallAndReturnsBallSuccessfully) {
  Timestamp now = TimeUtil::SecondsToTimestamp(k10Seconds);

  tp::TrackerWrapperPacket tracked_wrapper_packet = ParseTextOrDie(R"pb(
    uuid: "1df3fba7-adb9-40c4-a7ee-b0a559c40e4e"
    tracked_frame {
      frame_number: 42
      timestamp: 300
      balls {
        pos {
          x: 0.5
          y: 1.5
          z: 2
        }
        vel {
          x: 0.001
          y: 0.001
          z: 0.001
        }
        visibility: 1
      }
    }
  )pb");

  std::unique_ptr<TrackedDetectionMapper> mapper = makeMapper(now);

  rc::Detection detection = mapper->fromTrackedWrapperPacket(tracked_wrapper_packet);

  EXPECT_THAT(detection, EqualsProto(R"pb(
    created_at {
      seconds: 10
    }
    balls {
      confidence: 1
      position {
        x: 500
        y: 1500
        z: 2000
      }
      velocity {
        x: 1
        y: 1
        z: 1
      }
    }
  )pb"));
}

TEST_F(TrackedDetectionMapperTest, WhenMapperReceivesMultipleBallsAndReturnsBallsSuccessfully) {
  Timestamp now = TimeUtil::SecondsToTimestamp(k10Seconds);

  tp::TrackerWrapperPacket tracked_wrapper_packet = ParseTextOrDie(R"pb(
    uuid: "1df3fba7-adb9-40c4-a7ee-b0a559c40e4e"
    tracked_frame {
      frame_number: 42
      timestamp: 300
      balls {
        pos {
          x: 1
          y: 1
          z: 1
        }
        vel {
          x: 1
          y: 1
          z: 1
        }
        visibility: 0.5
      }
      balls {
        pos {
          x: 2
          y: 2
          z: 2
        }
        vel {
          x: 2
          y: 2
          z: 2
        }
        visibility: 1
      }
    }
  )pb");

  std::unique_ptr<TrackedDetectionMapper> mapper = makeMapper(now);

  rc::Detection detection = mapper->fromTrackedWrapperPacket(tracked_wrapper_packet);

  EXPECT_THAT(detection, EqualsProto(R"pb(
    created_at {
      seconds: 10
    }
    balls {
      confidence: 0.5
      position {
        x: 1000
        y: 1000
        z: 1000
      }
      velocity {
        x: 1000
        y: 1000
        z: 1000
      }
    }
    balls {
      confidence: 1
      position {
        x: 2000
        y: 2000
        z: 2000
      }
      velocity {
        x: 2000
        y: 2000
        z: 2000
      }
    }
  )pb"));
}

TEST_F(TrackedDetectionMapperTest, WhenMapperReceivesMultipleRobotsAndReturnsRobotsSuccessfully) {
  Timestamp now = TimeUtil::SecondsToTimestamp(k10Seconds);

  tp::TrackerWrapperPacket tracked_wrapper_packet = ParseTextOrDie(R"pb(
    uuid: "1df3fba7-adb9-40c4-a7ee-b0a559c40e4e"
    tracked_frame {
      frame_number: 42
      timestamp: 300
      robots {
        robot_id {
          id: 1
          team: BLUE
        }
        pos {
          x: 1
          y: 2
        }
        orientation: 0.5
        vel {
          x: 0.001
          y: 0.002
        }
        vel_angular: 0.5
        visibility: 0.5
      }
      robots {
        robot_id {
          id: 2
          team: YELLOW
        }
        pos {
          x: 3
          y: 4
        }
        orientation: 0.75
        vel {
          x: 0.003
          y: 0.004
        }
        vel_angular: 0.75
        visibility: 0.75
      }
    }
  )pb");

  std::unique_ptr<TrackedDetectionMapper> mapper = makeMapper(now);

  rc::Detection detection = mapper->fromTrackedWrapperPacket(tracked_wrapper_packet);

  EXPECT_THAT(detection, EqualsProto(R"pb(
    created_at {
      seconds: 10
    }
    robots {
      confidence: 0.5
      robot_id {
        color: COLOR_BLUE
        number: 1
      }
      position {
        x: 1000
        y: 2000
      }
      angle: 0.5
      velocity {
        x: 1
        y: 2
      }
      angular_velocity: 0.5
      radius: 90
      height: 140
      dribbler_width: 60
    }
    robots {
      confidence: 0.75
      robot_id {
        color: COLOR_YELLOW
        number: 2
      }
      position {
        x: 3000
        y: 4000
      }
      angle: 0.75
      velocity {
        x: 3
        y: 4
      }
      angular_velocity: 0.75
      radius: 90
      height: 140
      dribbler_width: 60
    }
  )pb"));
}

} // namespace perception

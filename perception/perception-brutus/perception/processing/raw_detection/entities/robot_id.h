#ifndef PERCEPTION_PROCESSING_RAW_DETECTION_ENTITIES_ROBOT_ID_H
#define PERCEPTION_PROCESSING_RAW_DETECTION_ENTITIES_ROBOT_ID_H

#include <protocols/third_party/game_controller/common.pb.h>

namespace perception {

using protocols::third_party::game_controller::BLUE;
using protocols::third_party::game_controller::RobotId;
using protocols::third_party::game_controller::YELLOW;

enum RobotColor {
  eColorUnspecified = 0,
  eColorYellow = 1,
  eColorBlue = 2
};

struct RobotID {
  uint32_t id;
  RobotColor color;

  bool operator==(const RobotID& other) const = default;
  bool operator!=(const RobotID& other) const = default;
  bool operator<(const RobotID& other) const {
    return id < other.id || (id == other.id && color < other.color);
  }
};

} // namespace perception

#endif // PERCEPTION_PROCESSING_RAW_DETECTION_ENTITIES_ROBOT_ID_H

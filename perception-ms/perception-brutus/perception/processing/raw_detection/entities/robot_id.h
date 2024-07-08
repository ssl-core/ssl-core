#ifndef PERCEPTION_PROCESSING_RAW_DETECTION_ENTITIES_ROBOT_ID_H
#define PERCEPTION_PROCESSING_RAW_DETECTION_ENTITIES_ROBOT_ID_H

#include <compare> // IWYU pragma: keep

namespace perception {

struct RobotId {
  enum class Color {
    Unspecified = 0,
    Yellow = 1,
    Blue = 2
  };

  int id;
  Color color;

  inline constexpr bool operator==(const RobotId& other) const = default;
  inline constexpr auto operator<=>(const RobotId& other) const = default;
};

} // namespace perception

#endif // PERCEPTION_PROCESSING_RAW_DETECTION_ENTITIES_ROBOT_ID_H

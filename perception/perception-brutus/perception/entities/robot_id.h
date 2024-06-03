#ifndef PERCEPTION_ENTITIES_ROBOT_ID_H
#define PERCEPTION_ENTITIES_ROBOT_ID_H

namespace perception {

enum RobotColor {
  eColorUnspecified = 0,
  eColorYellow = 1,
  eColorBlue = 2
};

struct RobotID {
  int id;
  RobotColor color;

  bool operator==(const RobotID& other) const { return id == other.id && color == other.color; }
  bool operator!=(const RobotID& other) const { return !(*this == other); }
  bool operator<(const RobotID& other) const {
    return id < other.id || (id == other.id && color < other.color);
  }
};

} // namespace perception

#endif // PERCEPTION_ENTITIES_ROBOT_ROBOT_ID_H

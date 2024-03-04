// Robot.h
#ifndef VISION_SRC_ROBOT_H
#define VISION_SRC_ROBOT_H

#include "vision/src/point/point_2df/point_2df.h"
#include "vision/src/robot/color/color.h"
#include "vision/src/source/source.h"

class Robot {
 public:
  Robot(std::string uuid,
        int id,
        const Color& color,
        const Point2Df& position,
        double angle,
        const Point2Df& velocity,
        double angular_velocity,
        const Source& source);

  friend std::ostream& operator<<(std::ostream& os, const Robot& robot);

  [[nodiscard]] inline std::string getUuid() const { return uuid_; };
  [[nodiscard]] inline int getId() const { return id_; };
  [[nodiscard]] inline Color getColor() const { return color_; };
  [[nodiscard]] inline Point2Df getPosition() const { return position_; };
  [[nodiscard]] inline double getAngle() const { return angle_; };
  [[nodiscard]] inline Point2Df getVelocity() const { return velocity_; };
  [[nodiscard]] inline double getAngularVelocity() const { return angular_velocity_; };
  [[nodiscard]] inline Source getSource() const { return source_; };

 private:
  std::string uuid_;
  int id_;
  Color color_;
  Point2Df position_;
  double angle_;
  Point2Df velocity_;
  double angular_velocity_;
  Source source_;
};

#endif // VISION_SRC_ROBOT_H

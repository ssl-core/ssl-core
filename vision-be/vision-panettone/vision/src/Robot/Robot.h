// Robot.h
#ifndef ROBOT_H
#define ROBOT_H

#include "../Point/Point2Df/Point2Df.h"
#include "../Source/Source.h"
#include "Color/Color.h"

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

#endif // ROBOT_H

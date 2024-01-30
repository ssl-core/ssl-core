// Ball.h
#ifndef VISION_SRC_BALL_H
#define VISION_SRC_BALL_H

#include "vision/src/point/point_2df/point_2df.h"
#include "vision/src/point/point_3df/point_3df.h"
#include "vision/src/source/source.h"

class Ball {
 public:
  Ball(std::string uuid, const Source& source, const Point2Df& position, const Point3Df& velocity);

  friend std::ostream& operator<<(std::ostream& os, const Ball& ball);

 private:
  std::string uuid_;
  Source source_;
  Point2Df position_;
  Point3Df velocity_;
};

#endif // VISION_SRC_BALL_H

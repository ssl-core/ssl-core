// Ball.h
#ifndef BALL_H
#define BALL_H

#include "../point/point_2df/point_2df.h"
#include "../point/point_3df/point_3df.h"
#include "../source/source.h"

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

#endif // BALL_H

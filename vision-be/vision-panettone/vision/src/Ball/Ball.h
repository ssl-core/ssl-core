// Ball.h
#ifndef BALL_H
#define BALL_H

#include "../Point/Point2Df/Point2Df.h"
#include "../Point/Point3Df/Point3Df.h"
#include "../Source/Source.h"

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

// Point2Df.h
#ifndef VISION_SRC_POINT2DF_H
#define VISION_SRC_POINT2DF_H

#include <iostream>

class Point2Df {
 public:
  Point2Df(double x, double y);

  friend std::ostream& operator<<(std::ostream& os, const Point2Df& point);

 private:
  double x_;
  double y_;
};

#endif // VISION_SRC_POINT2DF_H

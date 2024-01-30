// Point2Df.cpp
#include "vision/src/point/point_2df/point_2df.h"

// NOLINTBEGIN(bugprone-easily-swappable-parameters)
Point2Df::Point2Df(double x, double y) : x_(x), y_(y) {}
// NOLINTEND(bugprone-easily-swappable-parameters)

std::ostream& operator<<(std::ostream& os, const Point2Df& point) {
  os << "Point2Df(x=" << point.x_ << ", y=" << point.y_ << ")";
  return os;
}

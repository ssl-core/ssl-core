// Point2Df.cpp
#include "Point2Df.h"

Point2Df::Point2Df(double x, double y) : x_(x), y_(y) {}

std::ostream& operator<<(std::ostream& os, const Point2Df& point) {
  os << "Point2Df(x=" << point.x_ << ", y=" << point.y_ << ")";
  return os;
}

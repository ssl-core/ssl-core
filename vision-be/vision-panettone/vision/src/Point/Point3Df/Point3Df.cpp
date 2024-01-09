// Point3Df.cpp
#include "Point3Df.h"

Point3Df::Point3Df(double x, double y, double z) : x_(x), y_(y), z_(z) {}

std::ostream& operator<<(std::ostream& os, const Point3Df& point) {
  os << "Point3Df(x=" << point.x_ << ", y=" << point.y_ << ", z=" << point.z_ << ")";
  return os;
}

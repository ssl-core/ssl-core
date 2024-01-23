// Point3Df.cpp
#include "point_3df.h"

// NOLINTBEGIN(bugprone-easily-swappable-parameters)
Point3Df::Point3Df(double x, double y, double z) : x_(x), y_(y), z_(z) {}
// NOLINTEND(bugprone-easily-swappable-parameters)

std::ostream& operator<<(std::ostream& os, const Point3Df& point) {
  os << "Point3Df(x=" << point.x_ << ", y=" << point.y_ << ", z=" << point.z_ << ")";
  return os;
}

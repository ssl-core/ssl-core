// Point3Df.h
#ifndef VISION_SRC_POINT3DF_H
#define VISION_SRC_POINT3DF_H

#include <iostream>

class Point3Df {
 public:
  Point3Df(double x, double y, double z);

  friend std::ostream& operator<<(std::ostream& os, const Point3Df& point);

  [[nodiscard]] inline double getX() const { return x_; };
  [[nodiscard]] inline double getY() const { return y_; };
  [[nodiscard]] inline double getZ() const { return z_; };

 private:
  double x_;
  double y_;
  double z_;
};

#endif // VISION_SRC_POINT3DF_H

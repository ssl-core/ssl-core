#ifndef ROBOCIN_GEOMETRY_POINT2D_H
#define ROBOCIN_GEOMETRY_POINT2D_H

namespace robocin {

template <class T>
struct Point2D {
  T x, y;
};

using Point2Df = Point2D<float>;

} // namespace robocin

#endif // ROBOCIN_GEOMETRY_POINT2D_H

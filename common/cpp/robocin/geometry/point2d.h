#ifndef ROBOCIN_GEOMETRY_POINT2D_H
#define ROBOCIN_GEOMETRY_POINT2D_H

#include <type_traits>

namespace robocin {

template <class T>
struct Point2D {
  using value_type = T;

  value_type x, y;
};

Point2D() -> Point2D<double>;

template <class T, class U>
Point2D(T, U) -> Point2D<std::common_type_t<T, U>>;

using Point2Df = Point2D<float>;
using Point2Dd = Point2D<double>;
using Point2Di = Point2D<int>;

} // namespace robocin

#endif // ROBOCIN_GEOMETRY_POINT2D_H

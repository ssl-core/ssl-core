#ifndef ROBOCIN_GEOMETRY_POINT3D_H
#define ROBOCIN_GEOMETRY_POINT3D_H

#include <type_traits>

namespace robocin {

template <class T>
struct Point3D {
  using value_type = T;

  value_type x, y, z;
};

Point3D() -> Point3D<double>;

template <class T, class U, class V>
Point3D(T, U, V) -> Point3D<std::common_type_t<T, U, V>>;

using Point3Df = Point3D<float>;
using Point3Dd = Point3D<double>;
using Point3Di = Point3D<int>;

} // namespace robocin

#endif // ROBOCIN_GEOMETRY_POINT3D_H

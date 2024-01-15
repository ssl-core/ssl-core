#ifndef ROBOCIN_GEOMETRY_POINT3D_H
#define ROBOCIN_GEOMETRY_POINT3D_H

namespace robocin {

template <class T>
struct Point3D {
  T x, y, z;
};

using Point3Df = Point3D<float>;

} // namespace robocin

#endif // ROBOCIN_GEOMETRY_POINT3D_H

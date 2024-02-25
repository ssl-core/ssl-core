#include "robocin/geometry/point3d.h"

#include <cstdint>

namespace robocin {

template struct Point3D<std::int16_t>;
template struct Point3D<std::int32_t>;
template struct Point3D<std::int64_t>;
template struct Point3D<float>;
template struct Point3D<double>;
template struct Point3D<long double>;

} // namespace robocin

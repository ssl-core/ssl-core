#include "robocin/geometry/point2d.h"

#include <cstdint>

namespace robocin {

template struct Point2D<std::int16_t>;
template struct Point2D<std::int32_t>;
template struct Point2D<std::int64_t>;
template struct Point2D<float>;
template struct Point2D<double>;
template struct Point2D<long double>;

} // namespace robocin

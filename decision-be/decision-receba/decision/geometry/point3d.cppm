module;

#include <type_traits>
#include <utility>

export module decision.geometry:point3d;

namespace {

template <class PT>
concept StructPoint3D = requires(PT point) {
  point.x;
  point.y;
  point.z;
};

template <class PT>
concept ClassPoint3D = requires(PT point) {
  point.x();
  point.y();
  point.z();
};

} // namespace

export namespace decision {

template <class T>
struct Point3D {
  using value_type = T;

  constexpr Point3D() : x(0), y(0) {}
  constexpr Point3D(const Point3D& point) = default;
  constexpr Point3D(Point3D&& point) noexcept = default;
  // NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
  constexpr Point3D(value_type x, value_type y) : x(x), y(y) {}
  constexpr explicit Point3D(const StructPoint3D auto& point) :
      x(static_cast<value_type>(point.x)),
      y(static_cast<value_type>(point.y)) {}
  constexpr explicit Point3D(const ClassPoint3D auto& point) :
      x(static_cast<value_type>(point.x())),
      y(static_cast<value_type>(point.y())) {}
  template <class U, class V>
  constexpr explicit Point3D(const std::pair<U, V>& pair) :
      x(static_cast<value_type>(pair.first)),
      y(static_cast<value_type>(pair.second)) {}

  Point3D& operator=(const Point3D& point) = default;
  Point3D& operator=(Point3D&& point) noexcept = default;

  ~Point3D() = default;

  value_type x, y, z;
};

// Deduction guides --------------------------------------------------------------------------------
Point3D() -> Point3D<double>;

template <class T, class U, class V>
Point3D(T, U, V) -> Point3D<std::common_type_t<T, U, V>>;

// Aliases -----------------------------------------------------------------------------------------
using Point3Df = Point3D<float>;
using Point3Dd = Point3D<double>;
using Point3Dld = Point3D<long double>;
using Point3Di = Point3D<int>;

} // namespace decision

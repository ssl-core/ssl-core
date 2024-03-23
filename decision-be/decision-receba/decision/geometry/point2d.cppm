module;

#include <type_traits>
#include <utility>

export module decision.geometry:point2d;

namespace {

template <class PT>
concept StructPoint2D = requires(PT point) {
  point.x;
  point.y;
};

template <class PT>
concept ClassPoint2D = requires(PT point) {
  point.x();
  point.y();
};

} // namespace

export namespace decision {

template <class T>
struct Point2D {
  using value_type = T;

  constexpr Point2D() : x(0), y(0) {}
  constexpr Point2D(const Point2D& point) = default;
  constexpr Point2D(Point2D&& point) noexcept = default;
  // NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
  constexpr Point2D(value_type x, value_type y) : x(x), y(y) {}
  constexpr explicit Point2D(const StructPoint2D auto& point) :
      x(static_cast<value_type>(point.x)),
      y(static_cast<value_type>(point.y)) {}
  constexpr explicit Point2D(const ClassPoint2D auto& point) :
      x(static_cast<value_type>(point.x())),
      y(static_cast<value_type>(point.y())) {}
  template <class U, class V>
  constexpr explicit Point2D(const std::pair<U, V>& pair) :
      x(static_cast<value_type>(pair.first)),
      y(static_cast<value_type>(pair.second)) {}

  Point2D& operator=(const Point2D& point) = default;
  Point2D& operator=(Point2D&& point) noexcept = default;

  ~Point2D() = default;

  value_type x, y;
};

// Deduction guides --------------------------------------------------------------------------------
Point2D() -> Point2D<double>;

template <class T, class U>
Point2D(T, U) -> Point2D<std::common_type_t<T, U>>;

// Aliases -----------------------------------------------------------------------------------------
using Point2Df = Point2D<float>;
using Point2Dd = Point2D<double>;
using Point2Dld = Point2D<long double>;
using Point2Di = Point2D<int>;

} // namespace decision

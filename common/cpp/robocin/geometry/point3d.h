#ifndef ROBOCIN_GEOMETRY_POINT3D_H
#define ROBOCIN_GEOMETRY_POINT3D_H

#include "robocin/geometry/point2d.h"
#include "robocin/utility/internal/fuzzy_compare_internal.h"

#include <cmath>
#include <concepts>

namespace robocin {

template <class T>
struct Point3D {
 public:
  // Member types ----------------------------------------------------------------------------------
  using value_type = T;
  using reference = value_type&;
  // using size_type = std::size_t;
  // using iterator = point3d_internal::iterator<Point3D>;
  // using const_iterator = point3d_internal::iterator<const Point3D>;
  // using reverse_iterator = std::reverse_iterator<iterator>;
  // using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  // Friendships -----------------------------------------------------------------------------------
  template <class U>
  friend struct Point3D;

  // Members ---------------------------------------------------------------------------------------
  value_type x, y, z;

  // Static constructors ---------------------------------------------------------------------------
  static consteval Point3D origin() { return Point3D(); }

  // Constructors ----------------------------------------------------------------------------------
  constexpr Point3D() : x(0), y(0), z(0) {}
  constexpr Point3D(const Point3D& point) = default;
  constexpr Point3D(Point3D&& point) noexcept = default;
  constexpr Point3D(value_type x, value_type y, value_type z) : // NOLINT(*swappable*)
      x(x),
      y(y),
      z(z) {}
  template <class U>
  constexpr explicit Point3D(const Point2D<U> point) :
      x(static_cast<value_type>(point.x)),
      y(static_cast<value_type>(point.y)),
      z(0) {}
  // constexpr explicit Point3D(const point3d_internal::StructPoint auto& point) :
  //     x(static_cast<value_type>(point.x)),
  //     y(static_cast<value_type>(point.y)),
  //     z(static_cast<value_type>(point.z)) {}
  // constexpr explicit Point3D(const point3d_internal::ClassPoint auto& point) :
  //     x(static_cast<value_type>(point.x())),
  //     y(static_cast<value_type>(point.y())),
  //     z(static_cast<value_type>(point.z())) {}
  template <class U, class V>
  constexpr explicit Point3D(const std::pair<U, V>& pair) :
      x(static_cast<value_type>(pair.first)),
      y(static_cast<value_type>(pair.second)),
      z(0) {}

  // Destructor ------------------------------------------------------------------------------------
  constexpr ~Point3D() = default;

  // Assignment operators --------------------------------------------------------------------------
  constexpr Point3D& operator=(const Point3D& other) = default;
  constexpr Point3D& operator=(Point3D&& other) noexcept = default;

  // Validators ------------------------------------------------------------------------------------
  [[nodiscard]] constexpr bool isNull() const {
    return fuzzy_compare_internal::fuzzyIsZero(x) and fuzzy_compare_internal::fuzzyIsZero(y)
           and fuzzy_compare_internal::fuzzyIsZero(z);
  }

  // Arithmetic-assignment operators ---------------------------------------------------------------
  inline constexpr Point3D& operator+=(const Point3D& other) {
    return x += other.x, y += other.y, z += other.z, *this;
  }

  inline constexpr Point3D& operator-=(const Point3D& other) {
    return x -= other.x, y -= other.y, z -= other.z, *this;
  }

  inline constexpr Point3D& operator*=(value_type factor) {
    return x *= factor, y *= factor, z *= factor, *this;
  }

  inline constexpr Point3D& operator/=(value_type factor) {
    return x /= factor, y /= factor, z /= factor, *this;
  }

  // Arithmetic operators --------------------------------------------------------------------------
  inline constexpr Point3D operator+(const Point3D& other) const { return Point3D(*this) += other; }
  inline constexpr Point3D operator-(const Point3D& other) const { return Point3D(*this) -= other; }
  inline constexpr Point3D operator*(value_type factor) const { return Point3D(*this) *= factor; }
  inline constexpr Point3D operator/(value_type factor) const { return Point3D(*this) /= factor; }

  // Arithmetic friend operators -------------------------------------------------------------------
  friend inline constexpr Point3D operator*(value_type factor, const Point3D& point) {
    return Point3D(factor * point.x, factor * point.y, factor * point.z);
  }

  // Sign operators --------------------------------------------------------------------------------
  inline constexpr Point3D operator+() const { return *this; }
  inline constexpr Point3D operator-() const { return Point3D(-x, -y, -z); }

  // Comparison operators
  inline constexpr bool operator==(const Point3D& other) const {
    return fuzzy_compare_internal::fuzzyCmpEqual(x, other.x)
           and fuzzy_compare_internal::fuzzyCmpEqual(y, other.y)
           and fuzzy_compare_internal::fuzzyCmpEqual(z, other.z);
  }

  inline constexpr auto operator<=>(const Point3D& other) const {
    if (auto x_cmp = fuzzy_compare_internal::fuzzyCmpThreeWay(x, other.x); std::is_neq(x_cmp)) {
      return x_cmp;
    }
    if (auto y_cmp = fuzzy_compare_internal::fuzzyCmpThreeWay(y, other.y); std::is_neq(y_cmp)) {
      return y_cmp;
    }
    return fuzzy_compare_internal::fuzzyCmpThreeWay(z, other.z);
  }

  // Swap ------------------------------------------------------------------------------------------
  constexpr void swap(Point3D& other) noexcept { std::swap(*this, other); }
};

// Deduction guides --------------------------------------------------------------------------------
Point3D() -> Point3D<double>;

template <class T, class U, class V>
Point3D(T, U, V) -> Point3D<std::common_type_t<T, U, V>>;

// Aliases -----------------------------------------------------------------------------------------
using Point3Di = Point3D<std::int32_t>;
using Point3Df = Point3D<float>;
using Point3Dd = Point3D<double>;
using Point3Dld = Point3D<long double>;

} // namespace robocin

#endif // ROBOCIN_GEOMETRY_POINT3D_H

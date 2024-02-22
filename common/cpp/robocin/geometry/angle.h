#ifndef ROBOCIN_GEOMETRY_ANGLE_H
#define ROBOCIN_GEOMETRY_ANGLE_H

#include "robocin/geometry/internal/angle_internal.h"
#include "robocin/utility/angular.h"
#include "robocin/utility/concepts.h"
#include "robocin/utility/fuzzy_compare.h"
#include "robocin/utility/internal/fuzzy_compare_internal.h"
#include "robocin/utility/type_traits.h"

#include <concepts>
#include <iostream>
#include <numbers>

namespace robocin {

template <std::floating_point F>
class Angle {
 public:
  // Member types ----------------------------------------------------------------------------------
  using type = F;

  // Friendships -----------------------------------------------------------------------------------
  template <std::floating_point U>
  friend class Angle;

  // Static constructors ---------------------------------------------------------------------------
  static constexpr Angle fromDegrees(type degrees) {
    return Angle{degrees * kDegreesToRadiansFactor};
  }

  // Constructors ----------------------------------------------------------------------------------
  constexpr Angle() = default;
  constexpr Angle(const Angle&) = default;
  constexpr Angle(Angle&&) noexcept = default;

  // NOLINTBEGIN(google-explicit-constructor, hicpp-explicit-conversions)
  template <angle_internal::other_floating_point<type> G>
  constexpr Angle(const Angle<G>& other) : value_{static_cast<type>(other)} {}
  template <angle_internal::other_floating_point<type> G>
  constexpr Angle(Angle<G>&& other) noexcept : value_{static_cast<type>(std::move(other))} {}
  // NOLINTEND(google-explicit-constructor, hicpp-explicit-conversions)

  constexpr explicit Angle(type radians) : value_{radians} {}

  // Destructor ------------------------------------------------------------------------------------
  constexpr ~Angle() = default;

  // Implicit conversion operator ------------------------------------------------------------------
  // NOLINTBEGIN(google-explicit-constructor, hicpp-explicit-conversions)
  constexpr operator type() const { return value_; }
  // NOLINTEND(google-explicit-constructor, hicpp-explicit-conversions)

  // Assignment operators --------------------------------------------------------------------------
  // NOLINTBEGIN(cppcoreguidelines-c-copy-assignment-signature)
  // NOLINTBEGIN(misc-unconventional-assign-operator)
  constexpr Angle& operator=(type radians) { return value_ = radians, *this; }
  // NOLINTEND(misc-unconventional-assign-operator)
  // NOLINTEND(cppcoreguidelines-c-copy-assignment-signature)
  constexpr Angle& operator=(const Angle&) = default;
  constexpr Angle& operator=(Angle&&) noexcept = default;

  // Arithmetic-assignment operators ---------------------------------------------------------------
  constexpr Angle& operator+=(type scalar) { return value_ += scalar, *this; }
  constexpr Angle& operator-=(type scalar) { return value_ -= scalar, *this; }
  constexpr Angle& operator*=(type scalar) { return value_ *= scalar, *this; }
  constexpr Angle& operator/=(type scalar) { return value_ /= scalar, *this; }

  template <std::floating_point G>
  constexpr Angle& operator+=(const Angle<G>& other) {
    return value_ += other.value_, *this;
  }
  template <std::floating_point G>
  constexpr Angle& operator-=(const Angle<G>& other) {
    return value_ -= other.value_, *this;
  }
  template <std::floating_point G>
  constexpr Angle& operator*=(const Angle<G>& other) {
    return value_ *= other.value_, *this;
  }
  template <std::floating_point G>
  constexpr Angle& operator/=(const Angle<G>& other) {
    return value_ /= other.value_, *this;
  }

  // Arithmetic operators --------------------------------------------------------------------------
  [[nodiscard]] constexpr Angle operator+(type scalar) const { return Angle{value_ + scalar}; }
  [[nodiscard]] constexpr Angle operator-(type scalar) const { return Angle{value_ - scalar}; }
  [[nodiscard]] constexpr Angle operator*(type scalar) const { return Angle{value_ * scalar}; }
  [[nodiscard]] constexpr Angle operator/(type scalar) const { return Angle{value_ / scalar}; }

  template <std::floating_point G>
  [[nodiscard]] constexpr Angle operator+(const Angle<G>& other) const {
    return Angle{value_ + other.value_};
  }
  template <std::floating_point G>
  [[nodiscard]] constexpr Angle operator-(const Angle<G>& other) const {
    return Angle{value_ - other.value_};
  }
  template <std::floating_point G>
  [[nodiscard]] constexpr Angle operator*(const Angle<G>& other) const {
    return Angle{value_ * other.value_};
  }
  template <std::floating_point G>
  [[nodiscard]] constexpr Angle operator/(const Angle<G>& other) const {
    return Angle{value_ / other.value_};
  }

  // Arithmetic friend operators -------------------------------------------------------------------
  friend constexpr Angle operator*(type scalar, const Angle& angle) {
    return Angle{angle * scalar};
  }

  // Sign operators --------------------------------------------------------------------------------
  [[nodiscard]] constexpr Angle operator+() const { return *this; }
  [[nodiscard]] constexpr Angle operator-() const { return Angle{-value_}; }

  // Getters ---------------------------------------------------------------------------------------
  [[nodiscard]] constexpr type degrees() const { return value_ * kRadiansToDegreesFactor; }
  [[nodiscard]] constexpr type radians() const { return value_; }

  // Geometry --------------------------------------------------------------------------------------
  [[nodiscard]] constexpr type sin() const { return std::sin(value_); }
  [[nodiscard]] constexpr type cos() const { return std::cos(value_); }
  [[nodiscard]] constexpr type tan() const { return std::tan(value_); }

  constexpr void normalize() & { value_ = normalizeAngle(value_); }
  [[nodiscard]] constexpr Angle normalized() && { return normalize(), std::move(*this); }
  [[nodiscard]] constexpr Angle normalized() const& { return Angle{*this}.normalized(); }

  [[nodiscard]] constexpr Angle smallestAngleDiffTo(type other) && {
    return value_ = smallestAngleDiff(value_, other), std::move(*this);
  }
  [[nodiscard]] constexpr Angle smallestAngleDiffTo(type other) const& {
    return Angle{*this}.smallestAngleDiffTo(other);
  }

  [[nodiscard]] constexpr Angle absSmallestAngleDiffTo(type other) && {
    return value_ = absSmallestAngleDiff(value_, other), std::move(*this);
  }
  [[nodiscard]] constexpr Angle absSmallestAngleDiffTo(type other) const& {
    return Angle{*this}.absSmallestAngleDiffTo(other);
  }

  // Validators ------------------------------------------------------------------------------------
  [[nodiscard]] constexpr bool isNull() const {
    return fuzzy_compare_internal::fuzzyIsZero(value_);
  }

  // Equality operators ----------------------------------------------------------------------------
  template <std::floating_point G>
  [[nodiscard]] constexpr bool operator==(const Angle<G>& other) const {
    return fuzzy_compare_internal::fuzzyCmpEqual(value_, other.value_);
  }
  template <std::floating_point G>
  [[nodiscard]] constexpr bool operator==(G scalar) const {
    return fuzzy_compare_internal::fuzzyCmpEqual(value_, scalar);
  }

  // Equality friend operator ----------------------------------------------------------------------
  template <std::floating_point G>
  [[nodiscard]] friend constexpr bool operator==(G scalar, const Angle& angle) {
    return fuzzy_compare_internal::fuzzyCmpEqual(scalar, angle.value_);
  }

  // Three-way comparison operators ----------------------------------------------------------------
  template <std::floating_point G>
  [[nodiscard]] constexpr auto operator<=>(const Angle<G>& other) const {
    return fuzzy_compare_internal::fuzzyCmpThreeWay(value_, other.value_);
  }
  template <std::floating_point G>
  [[nodiscard]] constexpr auto operator<=>(G scalar) const {
    return fuzzy_compare_internal::fuzzyCmpThreeWay(value_, scalar);
  }

  // Three-way comparison friend operator ----------------------------------------------------------
  template <std::floating_point G>
  [[nodiscard]] friend constexpr auto operator<=>(G lhs, const Angle& rhs) {
    return fuzzy_compare_internal::fuzzyCmpThreeWay(lhs, rhs.value_);
  }

  // Input/Output operators ------------------------------------------------------------------------
  friend inline std::istream& operator>>(std::istream& is, Angle& angle) {
    return is >> angle.value_;
  }

  friend inline std::ostream& operator<<(std::ostream& os, const Angle& angle) {
    return os << angle.degrees() << "°";
  }

 private:
  static constexpr type kDegreesToRadiansFactor = std::numbers::pi_v<type> / static_cast<type>(180);
  static constexpr type kRadiansToDegreesFactor = static_cast<type>(180) / std::numbers::pi_v<type>;

  type value_{0};
};

// Deduction guides --------------------------------------------------------------------------------
template <arithmetic T>
Angle(T) -> Angle<std::conditional_t<std::is_floating_point_v<T>, T, double>>;

// Literals ----------------------------------------------------------------------------------------
inline namespace literals {
inline namespace angle_literals {

static consteval Angle<long double> operator"" _deg(long double degrees) {
  return Angle<long double>::fromDegrees(degrees);
}
static consteval Angle<long double> operator"" _deg(unsigned long long degrees) { // NOLINT(*int*)
  return Angle<long double>::fromDegrees(degrees);
}

static consteval Angle<long double> operator"" _rad(long double radians) { return Angle{radians}; }
static consteval Angle<long double> operator"" _rad(unsigned long long radians) { // NOLINT(*int*)
  return Angle{static_cast<long double>(radians)};
}

} // namespace angle_literals
} // namespace literals

} // namespace robocin

#endif // ROBOCIN_GEOMETRY_ANGLE_H

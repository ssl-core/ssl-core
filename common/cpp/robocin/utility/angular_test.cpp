#include "robocin/utility/angular.h"
#include "robocin/utility/internal/test/epsilon_injector.h"

#include <gtest/gtest.h>
#include <numbers>
#include <ranges>

namespace robocin {

using ::testing::Test;
using ::testing::Types;

using FloatingPointTestTypes = Types<float, double, long double>;

template <class>
class FloatingPointTest : public Test {};
TYPED_TEST_SUITE(FloatingPointTest, FloatingPointTestTypes);

// degreesToRadians --------------------------------------------------------------------------------
TYPED_TEST(FloatingPointTest, DegreesToRadiansGivenPiFractions) {
  using T = TypeParam;

  static constexpr T kEpsilon = epsilon_v<T>;
  static constexpr T kPi = std::numbers::pi_v<T>;
  static constexpr T kPiInDegrees = 180;

  for (const T kRatio : std::views::iota(0, 7)) {
    EXPECT_NEAR(degreesToRadians<T>(kRatio * kPiInDegrees), kRatio * kPi, kEpsilon);
  }
}

// radiansToDegrees --------------------------------------------------------------------------------
TYPED_TEST(FloatingPointTest, RadiansToDegreesGivenPiFractions) {
  using T = TypeParam;

  static constexpr T kEpsilon = epsilon_v<T>;
  static constexpr T kPi = std::numbers::pi_v<T>;
  static constexpr T kPiInDegrees = 180;

  for (const T kRatio : std::views::iota(0, 7)) {
    EXPECT_NEAR(radiansToDegrees<T>(kRatio * kPi), kRatio * kPiInDegrees, kEpsilon);
  }
}

// normalizeAngle ----------------------------------------------------------------------------------
TYPED_TEST(FloatingPointTest, NormalizeAngleGivenAnglesBetweenPiAndMinusPi) {
  using T = TypeParam;

  static constexpr T kEpsilon = epsilon_v<T>;
  static constexpr T kPi = std::numbers::pi_v<T>;

  EXPECT_NEAR(normalizeAngle<T>(0.0), 0.0, kEpsilon);           // 0.0 degrees
  EXPECT_NEAR(normalizeAngle<T>(kPi / 2), kPi / 2, kEpsilon);   // 90.0 degrees
  EXPECT_NEAR(normalizeAngle<T>(-kPi / 2), -kPi / 2, kEpsilon); // -90.0 degrees
  EXPECT_NEAR(normalizeAngle<T>(kPi), kPi, kEpsilon);           // 180.0 degrees
  EXPECT_NEAR(normalizeAngle<T>(-kPi), -kPi, kEpsilon);         // -180.0 degrees
}

TYPED_TEST(FloatingPointTest, NormalizeAngleGivenAnglesGreaterThanPi) {
  using T = TypeParam;

  static constexpr T kEpsilon = epsilon_v<T>;
  static constexpr T kPi = std::numbers::pi_v<T>;

  EXPECT_NEAR(normalizeAngle<T>(5 * kPi / 4), -3 * kPi / 4, kEpsilon); // 225.0 degrees
  EXPECT_NEAR(normalizeAngle<T>(-5 * kPi / 4), 3 * kPi / 4, kEpsilon); // -225.0 degrees
  EXPECT_NEAR(normalizeAngle<T>(3 * kPi / 2), -kPi / 2, kEpsilon);     // 270.0 degrees
  EXPECT_NEAR(normalizeAngle<T>(-3 * kPi / 2), kPi / 2, kEpsilon);     // -270.0 degrees
  EXPECT_NEAR(normalizeAngle<T>(7 * kPi / 3), kPi / 3, kEpsilon);      // 420.0 degrees
  EXPECT_NEAR(normalizeAngle<T>(-7 * kPi / 3), -kPi / 3, kEpsilon);    // -420.0 degrees
}

TYPED_TEST(FloatingPointTest, NormalizeAngleGivenAnglesLessThanMinusPi) {
  using T = TypeParam;

  static constexpr T kEpsilon = epsilon_v<T>;
  static constexpr T kPi = std::numbers::pi_v<T>;

  EXPECT_NEAR(normalizeAngle<T>(-9 * kPi / 4), -kPi / 4, kEpsilon); // -405.0 degrees
  EXPECT_NEAR(normalizeAngle<T>(9 * kPi / 4), kPi / 4, kEpsilon);   // 405.0 degrees
  EXPECT_NEAR(normalizeAngle<T>(-5 * kPi / 2), -kPi / 2, kEpsilon); // -450.0 degrees
  EXPECT_NEAR(normalizeAngle<T>(5 * kPi / 2), kPi / 2, kEpsilon);   // 450.0 degrees
  EXPECT_NEAR(normalizeAngle<T>(-11 * kPi / 3), kPi / 3, kEpsilon); // -660.0 degrees
  EXPECT_NEAR(normalizeAngle<T>(11 * kPi / 3), -kPi / 3, kEpsilon); // 660.0 degrees
}

// smallestAngleDiff -------------------------------------------------------------------------------
TYPED_TEST(FloatingPointTest, SmallestAngleDiffGivenAnglesBetweenPiAndMinusPi) {
  using T = TypeParam;

  static constexpr T kEpsilon = epsilon_v<T>;
  static constexpr T kPi = std::numbers::pi_v<T>;

  // 0.0 and 90.0 degrees
  EXPECT_NEAR((smallestAngleDiff<T, T>(0, kPi / 2)), kPi / 2, kEpsilon);
  // 90.0 and 0.0 degrees
  EXPECT_NEAR((smallestAngleDiff<T, T>(kPi / 2, 0)), -kPi / 2, kEpsilon);
  // -45.0 and 45.0 degrees
  EXPECT_NEAR((smallestAngleDiff<T, T>(-kPi / 4, kPi / 4)), kPi / 2, kEpsilon);
  // 45.0 and -45.0 degrees
  EXPECT_NEAR((smallestAngleDiff<T, T>(kPi / 4, -kPi / 4)), -kPi / 2, kEpsilon);
  // -45.0 and 135.0 degrees
  EXPECT_NEAR((smallestAngleDiff<T, T>(-kPi / 4, kPi / 2)), 3 * kPi / 4, kEpsilon);
  // 135.0 and -45.0 degrees
  EXPECT_NEAR((smallestAngleDiff<T, T>(kPi / 2, -kPi / 4)), -3 * kPi / 4, kEpsilon);
}

TYPED_TEST(FloatingPointTest, SmallestAngleDiffGivenAnglesOutsidePiAndMinusPi) {
  using T = TypeParam;

  static constexpr T kEpsilon = epsilon_v<T>;
  static constexpr T kPi = std::numbers::pi_v<T>;

  // -450.0 to 360.0 degrees
  EXPECT_NEAR((smallestAngleDiff<T, T>(-5 * kPi / 2, 2 * kPi)), kPi / 2, kEpsilon);
  // 360.0 to -450.0 degrees
  EXPECT_NEAR((smallestAngleDiff<T, T>(2 * kPi, -5 * kPi / 2)), -kPi / 2, kEpsilon);
  // -450.0 to 270.0 degrees
  EXPECT_NEAR((smallestAngleDiff<T, T>(-5 * kPi / 2, 3 * kPi / 2)), 0.0, kEpsilon);
  // -450.0 to 270.0 degrees
  EXPECT_NEAR((smallestAngleDiff<T, T>(3 * kPi / 2, -5 * kPi / 2)), 0.0, kEpsilon);
}

// absSmallestAngleDiff ----------------------------------------------------------------------------
TYPED_TEST(FloatingPointTest, AbsSmallestAngleDiffGivenAnglesBetweenPiAndMinusPi) {
  using T = TypeParam;

  static constexpr T kEpsilon = epsilon_v<T>;
  static constexpr T kPi = std::numbers::pi_v<T>;

  // 0.0 and 90.0 degrees
  EXPECT_NEAR((absSmallestAngleDiff<T, T>(0, kPi / 2)), kPi / 2, kEpsilon);
  // 90.0 and 0.0 degrees
  EXPECT_NEAR((absSmallestAngleDiff<T, T>(kPi / 2, 0)), kPi / 2, kEpsilon);
  // -45.0 and 45.0 degrees
  EXPECT_NEAR((absSmallestAngleDiff<T, T>(-kPi / 4, kPi / 4)), kPi / 2, kEpsilon);
  // 45.0 and -45.0 degrees
  EXPECT_NEAR((absSmallestAngleDiff<T, T>(kPi / 4, -kPi / 4)), kPi / 2, kEpsilon);
  // -45.0 and 135.0 degrees
  EXPECT_NEAR((absSmallestAngleDiff<T, T>(-kPi / 4, kPi / 2)), 3 * kPi / 4, kEpsilon);
  // 135.0 and -45.0 degrees
  EXPECT_NEAR((absSmallestAngleDiff<T, T>(kPi / 2, -kPi / 4)), 3 * kPi / 4, kEpsilon);
}

TYPED_TEST(FloatingPointTest, AbsSmallestAngleDiffGivenAnglesOutsidePiAndMinusPi) {
  using T = TypeParam;

  static constexpr T kEpsilon = epsilon_v<T>;
  static constexpr T kPi = std::numbers::pi_v<T>;

  // -450.0 to 360.0 degrees
  EXPECT_NEAR((absSmallestAngleDiff<T, T>(-5 * kPi / 2, 2 * kPi)), kPi / 2, kEpsilon);
  // 360.0 to -450.0 degrees
  EXPECT_NEAR((absSmallestAngleDiff<T, T>(2 * kPi, -5 * kPi / 2)), kPi / 2, kEpsilon);
  // -450.0 to 270.0 degrees
  EXPECT_NEAR((absSmallestAngleDiff<T, T>(-5 * kPi / 2, 3 * kPi / 2)), 0.0, kEpsilon);
  // -450.0 to 270.0 degrees
  EXPECT_NEAR((absSmallestAngleDiff<T, T>(3 * kPi / 2, -5 * kPi / 2)), 0.0, kEpsilon);
}

} // namespace robocin

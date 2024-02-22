#include "robocin/utility/fuzzy_compare.h"
#include "robocin/utility/internal/test/epsilon_injector.h"

#include <gtest/gtest.h>

namespace robocin {

using ::testing::Test;
using ::testing::Types;

using FloatingPointTestTypes = Types<float, double, long double>;

template <class>
class FloatingPointTest : public Test {};
TYPED_TEST_SUITE(FloatingPointTest, FloatingPointTestTypes);

// fuzzyIsZero -------------------------------------------------------------------------------------
TYPED_TEST(FloatingPointTest, FuzzyIsZeroGivenZeroValues) {
  using T = TypeParam;

  static constexpr T kEpsilon = epsilon_v<T>;

  EXPECT_TRUE((fuzzyIsZero<T, T>(0, kEpsilon)));
  EXPECT_TRUE((fuzzyIsZero<T, T>(0)));

  EXPECT_TRUE((fuzzyIsZero<T, T>(-0, kEpsilon)));
  EXPECT_TRUE((fuzzyIsZero<T, T>(-0)));
}

TYPED_TEST(FloatingPointTest, FuzzyIsZeroGivenLessThanEpsilonValues) {
  using T = TypeParam;

  static constexpr T kEpsilon = epsilon_v<T>;
  static constexpr T kQuarterEpsilon = epsilon_v<T> / 4;
  static constexpr T kHalfEpsilon = epsilon_v<T> / 2;
  static constexpr T kTwoThirdsEpsilon = 2 * epsilon_v<T> / 3;

  EXPECT_TRUE((fuzzyIsZero<T, T>(kQuarterEpsilon, kEpsilon)));
  EXPECT_TRUE((fuzzyIsZero<T, T>(kQuarterEpsilon)));

  EXPECT_TRUE((fuzzyIsZero<T, T>(-kQuarterEpsilon, kEpsilon)));
  EXPECT_TRUE((fuzzyIsZero<T, T>(-kQuarterEpsilon)));

  EXPECT_TRUE((fuzzyIsZero<T, T>(kHalfEpsilon, kEpsilon)));
  EXPECT_TRUE((fuzzyIsZero<T, T>(kHalfEpsilon)));

  EXPECT_TRUE((fuzzyIsZero<T, T>(-kHalfEpsilon, kEpsilon)));
  EXPECT_TRUE((fuzzyIsZero<T, T>(-kHalfEpsilon)));

  EXPECT_TRUE((fuzzyIsZero<T, T>(kTwoThirdsEpsilon, kEpsilon)));
  EXPECT_TRUE((fuzzyIsZero<T, T>(kTwoThirdsEpsilon)));

  EXPECT_TRUE((fuzzyIsZero<T, T>(-kTwoThirdsEpsilon, kEpsilon)));
  EXPECT_TRUE((fuzzyIsZero<T, T>(-kTwoThirdsEpsilon)));
}

TYPED_TEST(FloatingPointTest, FuzzyIsZeroGivenGreaterThanEpsilonValues) {
  using T = TypeParam;

  static constexpr T kEpsilon = epsilon_v<T>;
  static constexpr T kTwoEpsilon = 2 * epsilon_v<T>;
  static constexpr T kFortyTwo = 42;
  static constexpr T kLargeNumber = static_cast<T>(std::numeric_limits<int>::max()) / 2;

  EXPECT_FALSE((fuzzyIsZero<T, T>(kTwoEpsilon, kEpsilon)));
  EXPECT_FALSE((fuzzyIsZero<T, T>(kTwoEpsilon)));

  EXPECT_FALSE((fuzzyIsZero<T, T>(-kTwoEpsilon, kEpsilon)));
  EXPECT_FALSE((fuzzyIsZero<T, T>(-kTwoEpsilon)));

  EXPECT_FALSE((fuzzyIsZero<T, T>(kFortyTwo, kEpsilon)));
  EXPECT_FALSE((fuzzyIsZero<T, T>(kFortyTwo)));

  EXPECT_FALSE((fuzzyIsZero<T, T>(-kFortyTwo, kEpsilon)));
  EXPECT_FALSE((fuzzyIsZero<T, T>(-kFortyTwo)));

  EXPECT_FALSE((fuzzyIsZero<T, T>(kLargeNumber, kEpsilon)));
  EXPECT_FALSE((fuzzyIsZero<T, T>(kLargeNumber)));

  EXPECT_FALSE((fuzzyIsZero<T, T>(-kLargeNumber, kEpsilon)));
  EXPECT_FALSE((fuzzyIsZero<T, T>(-kLargeNumber)));
}

// fuzzyCmpEqual -----------------------------------------------------------------------------------
TYPED_TEST(FloatingPointTest, FuzzyCmpEqualGivenExactlyEqualValues) {
  using T = TypeParam;

  static constexpr T kEpsilon = epsilon_v<T>;
  static constexpr T kFortyTwo = 42;
  static constexpr T kLargeNumber = 99'999;

  EXPECT_TRUE((fuzzyCmpEqual<T, T, T>(0, 0, kEpsilon)));
  EXPECT_TRUE((fuzzyCmpEqual<T, T, T>(0, 0)));

  EXPECT_TRUE((fuzzyCmpEqual<T, T, T>(-0, -0, kEpsilon)));
  EXPECT_TRUE((fuzzyCmpEqual<T, T, T>(-0, -0)));

  EXPECT_TRUE((fuzzyCmpEqual<T, T, T>(kFortyTwo, kFortyTwo, kEpsilon)));
  EXPECT_TRUE((fuzzyCmpEqual<T, T, T>(kFortyTwo, kFortyTwo)));

  EXPECT_TRUE((fuzzyCmpEqual<T, T, T>(-kFortyTwo, -kFortyTwo, kEpsilon)));
  EXPECT_TRUE((fuzzyCmpEqual<T, T, T>(-kFortyTwo, -kFortyTwo)));

  EXPECT_TRUE((fuzzyCmpEqual<T, T, T>(kLargeNumber, kLargeNumber, kEpsilon)));
  EXPECT_TRUE((fuzzyCmpEqual<T, T, T>(kLargeNumber, kLargeNumber)));

  EXPECT_TRUE((fuzzyCmpEqual<T, T, T>(-kLargeNumber, -kLargeNumber, kEpsilon)));
  EXPECT_TRUE((fuzzyCmpEqual<T, T, T>(-kLargeNumber, -kLargeNumber)));
}

TYPED_TEST(FloatingPointTest, FuzzyCmpEqualGivenWithinEpsilonValues) {
  using T = TypeParam;

  static constexpr T kPi = std::numbers::pi_v<T>;
  static constexpr T kEpsilon = epsilon_v<T>;
  static constexpr T kHalfEpsilon = epsilon_v<T> / 2;

  EXPECT_TRUE((fuzzyCmpEqual<T, T, T>(0, kHalfEpsilon, kEpsilon)));
  EXPECT_TRUE((fuzzyCmpEqual<T, T, T>(0, kHalfEpsilon)));

  EXPECT_TRUE((fuzzyCmpEqual<T, T, T>(0, -kHalfEpsilon, kEpsilon)));
  EXPECT_TRUE((fuzzyCmpEqual<T, T, T>(0, -kHalfEpsilon)));

  EXPECT_TRUE((fuzzyCmpEqual<T, T, T>(kPi, kPi + kHalfEpsilon, kEpsilon)));
  EXPECT_TRUE((fuzzyCmpEqual<T, T, T>(kPi, kPi + kHalfEpsilon)));

  EXPECT_TRUE((fuzzyCmpEqual<T, T, T>(kPi, kPi - kHalfEpsilon, kEpsilon)));
  EXPECT_TRUE((fuzzyCmpEqual<T, T, T>(kPi, kPi - kHalfEpsilon)));
}

TYPED_TEST(FloatingPointTest, FuzzyCmpEqualGivenAlmostEqualButDifferentValues) {
  using T = TypeParam;

  static constexpr T kEpsilon = epsilon_v<T>;
  static constexpr T kTwoEpsilon = 2 * epsilon_v<T>;
  static constexpr T kFortyTwo = 42;
  static constexpr T kLargeNumber = 99'999;

  EXPECT_FALSE((fuzzyCmpEqual<T, T, T>(0, kTwoEpsilon, kEpsilon)));
  EXPECT_FALSE((fuzzyCmpEqual<T, T, T>(0, kTwoEpsilon)));

  EXPECT_FALSE((fuzzyCmpEqual<T, T, T>(0, -kTwoEpsilon, kEpsilon)));
  EXPECT_FALSE((fuzzyCmpEqual<T, T, T>(0, -kTwoEpsilon)));

  EXPECT_FALSE((fuzzyCmpEqual<T, T, T>(kFortyTwo, kFortyTwo + kTwoEpsilon, kEpsilon)));
  EXPECT_FALSE((fuzzyCmpEqual<T, T, T>(kFortyTwo, kFortyTwo + kTwoEpsilon)));

  EXPECT_FALSE((fuzzyCmpEqual<T, T, T>(kFortyTwo, kFortyTwo - kTwoEpsilon, kEpsilon)));
  EXPECT_FALSE((fuzzyCmpEqual<T, T, T>(kFortyTwo, kFortyTwo - kTwoEpsilon)));

  EXPECT_FALSE((fuzzyCmpEqual<T, T, T>(kLargeNumber, kLargeNumber + kTwoEpsilon, kEpsilon)));
  EXPECT_FALSE((fuzzyCmpEqual<T, T, T>(kLargeNumber, kLargeNumber + kTwoEpsilon)));

  EXPECT_FALSE((fuzzyCmpEqual<T, T, T>(kLargeNumber, kLargeNumber - kTwoEpsilon, kEpsilon)));
  EXPECT_FALSE((fuzzyCmpEqual<T, T, T>(kLargeNumber, kLargeNumber - kTwoEpsilon)));
}

TYPED_TEST(FloatingPointTest, FuzzyCmpEqualGivenDifferentValues) {
  using T = TypeParam;

  static constexpr T kEpsilon = epsilon_v<T>;
  static constexpr T kFortyTwo = 42;
  static constexpr T kSeven = 7;

  EXPECT_FALSE((fuzzyCmpEqual<T, T, T>(0, kFortyTwo, kEpsilon)));
  EXPECT_FALSE((fuzzyCmpEqual<T, T, T>(0, kFortyTwo)));

  EXPECT_FALSE((fuzzyCmpEqual<T, T, T>(0, -kFortyTwo, kEpsilon)));
  EXPECT_FALSE((fuzzyCmpEqual<T, T, T>(0, -kFortyTwo)));

  EXPECT_FALSE((fuzzyCmpEqual<T, T, T>(kSeven, kFortyTwo, kEpsilon)));
  EXPECT_FALSE((fuzzyCmpEqual<T, T, T>(kSeven, kFortyTwo)));

  EXPECT_FALSE((fuzzyCmpEqual<T, T, T>(kSeven, -kFortyTwo, kEpsilon)));
  EXPECT_FALSE((fuzzyCmpEqual<T, T, T>(kSeven, -kFortyTwo)));
}

// fuzzyCmpNotEqual --------------------------------------------------------------------------------
TYPED_TEST(FloatingPointTest, FuzzyCmpNotEqualGivenExactlyEqualValues) {
  using T = TypeParam;

  static constexpr T kEpsilon = epsilon_v<T>;
  static constexpr T kFortyTwo = 42;
  static constexpr T kLargeNumber = 99'999;

  EXPECT_FALSE((fuzzyCmpNotEqual<T, T, T>(0, 0, kEpsilon)));
  EXPECT_FALSE((fuzzyCmpNotEqual<T, T, T>(0, 0)));

  EXPECT_FALSE((fuzzyCmpNotEqual<T, T, T>(-0, -0, kEpsilon)));
  EXPECT_FALSE((fuzzyCmpNotEqual<T, T, T>(-0, -0)));

  EXPECT_FALSE((fuzzyCmpNotEqual<T, T, T>(kFortyTwo, kFortyTwo, kEpsilon)));
  EXPECT_FALSE((fuzzyCmpNotEqual<T, T, T>(kFortyTwo, kFortyTwo)));

  EXPECT_FALSE((fuzzyCmpNotEqual<T, T, T>(-kFortyTwo, -kFortyTwo, kEpsilon)));
  EXPECT_FALSE((fuzzyCmpNotEqual<T, T, T>(-kFortyTwo, -kFortyTwo)));

  EXPECT_FALSE((fuzzyCmpNotEqual<T, T, T>(kLargeNumber, kLargeNumber, kEpsilon)));
  EXPECT_FALSE((fuzzyCmpNotEqual<T, T, T>(kLargeNumber, kLargeNumber)));

  EXPECT_FALSE((fuzzyCmpNotEqual<T, T, T>(-kLargeNumber, -kLargeNumber, kEpsilon)));
  EXPECT_FALSE((fuzzyCmpNotEqual<T, T, T>(-kLargeNumber, -kLargeNumber)));
}

TYPED_TEST(FloatingPointTest, FuzzyCmpNotEqualGivenWithinEpsilonValues) {
  using T = TypeParam;

  static constexpr T kPi = std::numbers::pi_v<T>;
  static constexpr T kEpsilon = epsilon_v<T>;
  static constexpr T kHalfEpsilon = epsilon_v<T> / 2;

  EXPECT_FALSE((fuzzyCmpNotEqual<T, T, T>(0, kHalfEpsilon, kEpsilon)));
  EXPECT_FALSE((fuzzyCmpNotEqual<T, T, T>(0, kHalfEpsilon)));

  EXPECT_FALSE((fuzzyCmpNotEqual<T, T, T>(0, -kHalfEpsilon, kEpsilon)));
  EXPECT_FALSE((fuzzyCmpNotEqual<T, T, T>(0, -kHalfEpsilon)));

  EXPECT_FALSE((fuzzyCmpNotEqual<T, T, T>(kPi, kPi + kHalfEpsilon, kEpsilon)));
  EXPECT_FALSE((fuzzyCmpNotEqual<T, T, T>(kPi, kPi + kHalfEpsilon)));

  EXPECT_FALSE((fuzzyCmpNotEqual<T, T, T>(kPi, kPi - kHalfEpsilon, kEpsilon)));
  EXPECT_FALSE((fuzzyCmpNotEqual<T, T, T>(kPi, kPi - kHalfEpsilon)));
}

TYPED_TEST(FloatingPointTest, FuzzyCmpNotEqualGivenAlmostEqualButDifferentValues) {
  using T = TypeParam;

  static constexpr T kEpsilon = epsilon_v<T>;
  static constexpr T kTwoEpsilon = 2 * epsilon_v<T>;
  static constexpr T kFortyTwo = 42;
  static constexpr T kLargeNumber = 99'999;

  EXPECT_TRUE((fuzzyCmpNotEqual<T, T, T>(0, kTwoEpsilon, kEpsilon)));
  EXPECT_TRUE((fuzzyCmpNotEqual<T, T, T>(0, kTwoEpsilon)));

  EXPECT_TRUE((fuzzyCmpNotEqual<T, T, T>(0, -kTwoEpsilon, kEpsilon)));
  EXPECT_TRUE((fuzzyCmpNotEqual<T, T, T>(0, -kTwoEpsilon)));

  EXPECT_TRUE((fuzzyCmpNotEqual<T, T, T>(kFortyTwo, kFortyTwo + kTwoEpsilon, kEpsilon)));
  EXPECT_TRUE((fuzzyCmpNotEqual<T, T, T>(kFortyTwo, kFortyTwo + kTwoEpsilon)));

  EXPECT_TRUE((fuzzyCmpNotEqual<T, T, T>(kFortyTwo, kFortyTwo - kTwoEpsilon, kEpsilon)));
  EXPECT_TRUE((fuzzyCmpNotEqual<T, T, T>(kFortyTwo, kFortyTwo - kTwoEpsilon)));

  EXPECT_TRUE((fuzzyCmpNotEqual<T, T, T>(kLargeNumber, kLargeNumber + kTwoEpsilon, kEpsilon)));
  EXPECT_TRUE((fuzzyCmpNotEqual<T, T, T>(kLargeNumber, kLargeNumber + kTwoEpsilon)));

  EXPECT_TRUE((fuzzyCmpNotEqual<T, T, T>(kLargeNumber, kLargeNumber - kTwoEpsilon, kEpsilon)));
  EXPECT_TRUE((fuzzyCmpNotEqual<T, T, T>(kLargeNumber, kLargeNumber - kTwoEpsilon)));
}

TYPED_TEST(FloatingPointTest, FuzzyCmpNotEqualGivenDifferentValues) {
  using T = TypeParam;

  static constexpr T kEpsilon = epsilon_v<T>;
  static constexpr T kFortyTwo = 42;
  static constexpr T kSeven = 7;

  EXPECT_TRUE((fuzzyCmpNotEqual<T, T, T>(0, kFortyTwo, kEpsilon)));
  EXPECT_TRUE((fuzzyCmpNotEqual<T, T, T>(0, kFortyTwo)));

  EXPECT_TRUE((fuzzyCmpNotEqual<T, T, T>(0, -kFortyTwo, kEpsilon)));
  EXPECT_TRUE((fuzzyCmpNotEqual<T, T, T>(0, -kFortyTwo)));

  EXPECT_TRUE((fuzzyCmpNotEqual<T, T, T>(kSeven, kFortyTwo, kEpsilon)));
  EXPECT_TRUE((fuzzyCmpNotEqual<T, T, T>(kSeven, kFortyTwo)));

  EXPECT_TRUE((fuzzyCmpNotEqual<T, T, T>(kSeven, -kFortyTwo, kEpsilon)));
  EXPECT_TRUE((fuzzyCmpNotEqual<T, T, T>(kSeven, -kFortyTwo)));
}

// fuzzyCmpThreeWay --------------------------------------------------------------------------------
TYPED_TEST(FloatingPointTest, FuzzyCmpThreeWayGivenEqualValues) {
  using T = TypeParam;

  static constexpr T kEpsilon = epsilon_v<T>;
  static constexpr T kFortyTwo = 42;
  static constexpr T kLargeNumber = 99'999;

  EXPECT_TRUE(std::is_eq((fuzzyCmpThreeWay<T, T, T>(0, 0, kEpsilon))));
  EXPECT_TRUE(std::is_eq((fuzzyCmpThreeWay<T, T, T>(0, 0))));

  EXPECT_TRUE(std::is_eq((fuzzyCmpThreeWay<T, T, T>(-0, -0, kEpsilon))));
  EXPECT_TRUE(std::is_eq((fuzzyCmpThreeWay<T, T, T>(-0, -0))));

  EXPECT_TRUE(std::is_eq((fuzzyCmpThreeWay<T, T, T>(kFortyTwo, kFortyTwo, kEpsilon))));
  EXPECT_TRUE(std::is_eq((fuzzyCmpThreeWay<T, T, T>(kFortyTwo, kFortyTwo))));

  EXPECT_TRUE(std::is_eq((fuzzyCmpThreeWay<T, T, T>(-kFortyTwo, -kFortyTwo, kEpsilon))));
  EXPECT_TRUE(std::is_eq((fuzzyCmpThreeWay<T, T, T>(-kFortyTwo, -kFortyTwo))));

  EXPECT_TRUE(std::is_eq((fuzzyCmpThreeWay<T, T, T>(kLargeNumber, kLargeNumber, kEpsilon))));
  EXPECT_TRUE(std::is_eq((fuzzyCmpThreeWay<T, T, T>(kLargeNumber, kLargeNumber))));

  EXPECT_TRUE(std::is_eq((fuzzyCmpThreeWay<T, T, T>(-kLargeNumber, -kLargeNumber, kEpsilon))));
  EXPECT_TRUE(std::is_eq((fuzzyCmpThreeWay<T, T, T>(-kLargeNumber, -kLargeNumber))));
}

TYPED_TEST(FloatingPointTest, FuzzyCmpThreeWayGivenTheFirstValueLessThanTheSecond) {
  using T = TypeParam;

  static constexpr T kEpsilon = epsilon_v<T>;
  static constexpr T kFortyTwo = 42;
  static constexpr T kLargeNumber = 99'999;

  EXPECT_TRUE(std::is_lt((fuzzyCmpThreeWay<T, T, T>(0, kFortyTwo, kEpsilon))));
  EXPECT_TRUE(std::is_lt((fuzzyCmpThreeWay<T, T, T>(0, kFortyTwo))));

  EXPECT_TRUE(std::is_lt((fuzzyCmpThreeWay<T, T, T>(-kFortyTwo, 0, kEpsilon))));
  EXPECT_TRUE(std::is_lt((fuzzyCmpThreeWay<T, T, T>(-kFortyTwo, 0))));

  EXPECT_TRUE(std::is_lt((fuzzyCmpThreeWay<T, T, T>(kFortyTwo, kLargeNumber, kEpsilon))));
  EXPECT_TRUE(std::is_lt((fuzzyCmpThreeWay<T, T, T>(kFortyTwo, kLargeNumber))));

  EXPECT_TRUE(std::is_lt((fuzzyCmpThreeWay<T, T, T>(-kLargeNumber, -kFortyTwo, kEpsilon))));
  EXPECT_TRUE(std::is_lt((fuzzyCmpThreeWay<T, T, T>(-kLargeNumber, -kFortyTwo))));
}

TYPED_TEST(FloatingPointTest, FuzzyCmpThreeWayGivenTheFirstValueGreaterThanTheSecond) {
  using T = TypeParam;

  static constexpr T kEpsilon = epsilon_v<T>;
  static constexpr T kFortyTwo = 42;
  static constexpr T kLargeNumber = 99'999;

  EXPECT_TRUE(std::is_gt((fuzzyCmpThreeWay<T, T, T>(kFortyTwo, 0, kEpsilon))));
  EXPECT_TRUE(std::is_gt((fuzzyCmpThreeWay<T, T, T>(kFortyTwo, 0))));

  EXPECT_TRUE(std::is_gt((fuzzyCmpThreeWay<T, T, T>(0, -kFortyTwo, kEpsilon))));
  EXPECT_TRUE(std::is_gt((fuzzyCmpThreeWay<T, T, T>(0, -kFortyTwo))));

  EXPECT_TRUE(std::is_gt((fuzzyCmpThreeWay<T, T, T>(kLargeNumber, kFortyTwo, kEpsilon))));
  EXPECT_TRUE(std::is_gt((fuzzyCmpThreeWay<T, T, T>(kLargeNumber, kFortyTwo))));

  EXPECT_TRUE(std::is_gt((fuzzyCmpThreeWay<T, T, T>(-kFortyTwo, -kLargeNumber, kEpsilon))));
  EXPECT_TRUE(std::is_gt((fuzzyCmpThreeWay<T, T, T>(-kFortyTwo, -kLargeNumber))));
}

// fuzzyCmpLess ------------------------------------------------------------------------------------
TYPED_TEST(FloatingPointTest, FuzzyCmpLessGivenEqualValues) {
  using T = TypeParam;

  static constexpr T kEpsilon = epsilon_v<T>;
  static constexpr T kFortyTwo = 42;
  static constexpr T kLargeNumber = 99'999;

  EXPECT_FALSE((fuzzyCmpLess<T, T, T>(0, 0, kEpsilon)));
  EXPECT_FALSE((fuzzyCmpLess<T, T, T>(0, 0)));

  EXPECT_FALSE((fuzzyCmpLess<T, T, T>(-0, -0, kEpsilon)));
  EXPECT_FALSE((fuzzyCmpLess<T, T, T>(-0, -0)));

  EXPECT_FALSE((fuzzyCmpLess<T, T, T>(kFortyTwo, kFortyTwo, kEpsilon)));
  EXPECT_FALSE((fuzzyCmpLess<T, T, T>(kFortyTwo, kFortyTwo)));

  EXPECT_FALSE((fuzzyCmpLess<T, T, T>(-kFortyTwo, -kFortyTwo, kEpsilon)));
  EXPECT_FALSE((fuzzyCmpLess<T, T, T>(-kFortyTwo, -kFortyTwo)));

  EXPECT_FALSE((fuzzyCmpLess<T, T, T>(kLargeNumber, kLargeNumber, kEpsilon)));
  EXPECT_FALSE((fuzzyCmpLess<T, T, T>(kLargeNumber, kLargeNumber)));

  EXPECT_FALSE((fuzzyCmpLess<T, T, T>(-kLargeNumber, -kLargeNumber, kEpsilon)));
  EXPECT_FALSE((fuzzyCmpLess<T, T, T>(-kLargeNumber, -kLargeNumber)));
}

TYPED_TEST(FloatingPointTest, FuzzyCmpLessGivenTheFirstValueLessThanTheSecond) {
  using T = TypeParam;

  static constexpr T kEpsilon = epsilon_v<T>;
  static constexpr T kFortyTwo = 42;
  static constexpr T kLargeNumber = 99'999;

  EXPECT_TRUE((fuzzyCmpLess<T, T, T>(0, kFortyTwo, kEpsilon)));
  EXPECT_TRUE((fuzzyCmpLess<T, T, T>(0, kFortyTwo)));

  EXPECT_TRUE((fuzzyCmpLess<T, T, T>(-kFortyTwo, 0, kEpsilon)));
  EXPECT_TRUE((fuzzyCmpLess<T, T, T>(-kFortyTwo, 0)));

  EXPECT_TRUE((fuzzyCmpLess<T, T, T>(kFortyTwo, kLargeNumber, kEpsilon)));
  EXPECT_TRUE((fuzzyCmpLess<T, T, T>(kFortyTwo, kLargeNumber)));

  EXPECT_TRUE((fuzzyCmpLess<T, T, T>(-kLargeNumber, -kFortyTwo, kEpsilon)));
  EXPECT_TRUE((fuzzyCmpLess<T, T, T>(-kLargeNumber, -kFortyTwo)));
}

TYPED_TEST(FloatingPointTest, FuzzyCmpLessGivenTheFirstValueGreaterThanTheSecond) {
  using T = TypeParam;

  static constexpr T kEpsilon = epsilon_v<T>;
  static constexpr T kFortyTwo = 42;
  static constexpr T kLargeNumber = 99'999;

  EXPECT_FALSE((fuzzyCmpLess<T, T, T>(kFortyTwo, 0, kEpsilon)));
  EXPECT_FALSE((fuzzyCmpLess<T, T, T>(kFortyTwo, 0)));

  EXPECT_FALSE((fuzzyCmpLess<T, T, T>(0, -kFortyTwo, kEpsilon)));
  EXPECT_FALSE((fuzzyCmpLess<T, T, T>(0, -kFortyTwo)));

  EXPECT_FALSE((fuzzyCmpLess<T, T, T>(kLargeNumber, kFortyTwo, kEpsilon)));
  EXPECT_FALSE((fuzzyCmpLess<T, T, T>(kLargeNumber, kFortyTwo)));

  EXPECT_FALSE((fuzzyCmpLess<T, T, T>(-kFortyTwo, -kLargeNumber, kEpsilon)));
  EXPECT_FALSE((fuzzyCmpLess<T, T, T>(-kFortyTwo, -kLargeNumber)));
}

// fuzzyCmpLessEqual -------------------------------------------------------------------------------
TYPED_TEST(FloatingPointTest, FuzzyCmpLessEqualGivenEqualValues) {
  using T = TypeParam;

  static constexpr T kEpsilon = epsilon_v<T>;
  static constexpr T kFortyTwo = 42;
  static constexpr T kLargeNumber = 99'999;

  EXPECT_TRUE((fuzzyCmpLessEqual<T, T, T>(0, 0, kEpsilon)));
  EXPECT_TRUE((fuzzyCmpLessEqual<T, T, T>(0, 0)));

  EXPECT_TRUE((fuzzyCmpLessEqual<T, T, T>(-0, -0, kEpsilon)));
  EXPECT_TRUE((fuzzyCmpLessEqual<T, T, T>(-0, -0)));

  EXPECT_TRUE((fuzzyCmpLessEqual<T, T, T>(kFortyTwo, kFortyTwo, kEpsilon)));
  EXPECT_TRUE((fuzzyCmpLessEqual<T, T, T>(kFortyTwo, kFortyTwo)));

  EXPECT_TRUE((fuzzyCmpLessEqual<T, T, T>(-kFortyTwo, -kFortyTwo, kEpsilon)));
  EXPECT_TRUE((fuzzyCmpLessEqual<T, T, T>(-kFortyTwo, -kFortyTwo)));

  EXPECT_TRUE((fuzzyCmpLessEqual<T, T, T>(kLargeNumber, kLargeNumber, kEpsilon)));
  EXPECT_TRUE((fuzzyCmpLessEqual<T, T, T>(kLargeNumber, kLargeNumber)));

  EXPECT_TRUE((fuzzyCmpLessEqual<T, T, T>(-kLargeNumber, -kLargeNumber, kEpsilon)));
  EXPECT_TRUE((fuzzyCmpLessEqual<T, T, T>(-kLargeNumber, -kLargeNumber)));
}

TYPED_TEST(FloatingPointTest, FuzzyCmpLessEqualGivenTheFirstValueLessThanTheSecond) {
  using T = TypeParam;

  static constexpr T kEpsilon = epsilon_v<T>;
  static constexpr T kFortyTwo = 42;
  static constexpr T kLargeNumber = 99'999;

  EXPECT_TRUE((fuzzyCmpLessEqual<T, T, T>(0, kFortyTwo, kEpsilon)));
  EXPECT_TRUE((fuzzyCmpLessEqual<T, T, T>(0, kFortyTwo)));

  EXPECT_TRUE((fuzzyCmpLessEqual<T, T, T>(-kFortyTwo, 0, kEpsilon)));
  EXPECT_TRUE((fuzzyCmpLessEqual<T, T, T>(-kFortyTwo, 0)));

  EXPECT_TRUE((fuzzyCmpLessEqual<T, T, T>(kFortyTwo, kLargeNumber, kEpsilon)));
  EXPECT_TRUE((fuzzyCmpLessEqual<T, T, T>(kFortyTwo, kLargeNumber)));

  EXPECT_TRUE((fuzzyCmpLessEqual<T, T, T>(-kLargeNumber, -kFortyTwo, kEpsilon)));
  EXPECT_TRUE((fuzzyCmpLessEqual<T, T, T>(-kLargeNumber, -kFortyTwo)));
}

TYPED_TEST(FloatingPointTest, FuzzyCmpLessEqualGivenTheFirstValueGreaterThanTheSecond) {
  using T = TypeParam;

  static constexpr T kEpsilon = epsilon_v<T>;
  static constexpr T kFortyTwo = 42;
  static constexpr T kLargeNumber = 99'999;

  EXPECT_FALSE((fuzzyCmpLessEqual<T, T, T>(kFortyTwo, 0, kEpsilon)));
  EXPECT_FALSE((fuzzyCmpLessEqual<T, T, T>(kFortyTwo, 0)));

  EXPECT_FALSE((fuzzyCmpLessEqual<T, T, T>(0, -kFortyTwo, kEpsilon)));
  EXPECT_FALSE((fuzzyCmpLessEqual<T, T, T>(0, -kFortyTwo)));

  EXPECT_FALSE((fuzzyCmpLessEqual<T, T, T>(kLargeNumber, kFortyTwo, kEpsilon)));
  EXPECT_FALSE((fuzzyCmpLessEqual<T, T, T>(kLargeNumber, kFortyTwo)));

  EXPECT_FALSE((fuzzyCmpLessEqual<T, T, T>(-kFortyTwo, -kLargeNumber, kEpsilon)));
  EXPECT_FALSE((fuzzyCmpLessEqual<T, T, T>(-kFortyTwo, -kLargeNumber)));
}

// fuzzyCmpGreater ---------------------------------------------------------------------------------
TYPED_TEST(FloatingPointTest, FuzzyCmpGreaterGivenEqualValues) {
  using T = TypeParam;

  static constexpr T kEpsilon = epsilon_v<T>;
  static constexpr T kFortyTwo = 42;
  static constexpr T kLargeNumber = 99'999;

  EXPECT_FALSE((fuzzyCmpGreater<T, T, T>(0, 0, kEpsilon)));
  EXPECT_FALSE((fuzzyCmpGreater<T, T, T>(0, 0)));

  EXPECT_FALSE((fuzzyCmpGreater<T, T, T>(-0, -0, kEpsilon)));
  EXPECT_FALSE((fuzzyCmpGreater<T, T, T>(-0, -0)));

  EXPECT_FALSE((fuzzyCmpGreater<T, T, T>(kFortyTwo, kFortyTwo, kEpsilon)));
  EXPECT_FALSE((fuzzyCmpGreater<T, T, T>(kFortyTwo, kFortyTwo)));

  EXPECT_FALSE((fuzzyCmpGreater<T, T, T>(-kFortyTwo, -kFortyTwo, kEpsilon)));
  EXPECT_FALSE((fuzzyCmpGreater<T, T, T>(-kFortyTwo, -kFortyTwo)));

  EXPECT_FALSE((fuzzyCmpGreater<T, T, T>(kLargeNumber, kLargeNumber, kEpsilon)));
  EXPECT_FALSE((fuzzyCmpGreater<T, T, T>(kLargeNumber, kLargeNumber)));

  EXPECT_FALSE((fuzzyCmpGreater<T, T, T>(-kLargeNumber, -kLargeNumber, kEpsilon)));
  EXPECT_FALSE((fuzzyCmpGreater<T, T, T>(-kLargeNumber, -kLargeNumber)));
}

TYPED_TEST(FloatingPointTest, FuzzyCmpGreaterGivenTheFirstValueLessThanTheSecond) {
  using T = TypeParam;

  static constexpr T kEpsilon = epsilon_v<T>;
  static constexpr T kFortyTwo = 42;
  static constexpr T kLargeNumber = 99'999;

  EXPECT_FALSE((fuzzyCmpGreater<T, T, T>(0, kFortyTwo, kEpsilon)));
  EXPECT_FALSE((fuzzyCmpGreater<T, T, T>(0, kFortyTwo)));

  EXPECT_FALSE((fuzzyCmpGreater<T, T, T>(-kFortyTwo, 0, kEpsilon)));
  EXPECT_FALSE((fuzzyCmpGreater<T, T, T>(-kFortyTwo, 0)));

  EXPECT_FALSE((fuzzyCmpGreater<T, T, T>(kFortyTwo, kLargeNumber, kEpsilon)));
  EXPECT_FALSE((fuzzyCmpGreater<T, T, T>(kFortyTwo, kLargeNumber)));

  EXPECT_FALSE((fuzzyCmpGreater<T, T, T>(-kLargeNumber, -kFortyTwo, kEpsilon)));
  EXPECT_FALSE((fuzzyCmpGreater<T, T, T>(-kLargeNumber, -kFortyTwo)));
}

TYPED_TEST(FloatingPointTest, FuzzyCmpGreaterGivenTheFirstValueGreaterThanTheSecond) {
  using T = TypeParam;

  static constexpr T kEpsilon = epsilon_v<T>;
  static constexpr T kFortyTwo = 42;
  static constexpr T kLargeNumber = 99'999;

  EXPECT_TRUE((fuzzyCmpGreater<T, T, T>(kFortyTwo, 0, kEpsilon)));
  EXPECT_TRUE((fuzzyCmpGreater<T, T, T>(kFortyTwo, 0)));

  EXPECT_TRUE((fuzzyCmpGreater<T, T, T>(0, -kFortyTwo, kEpsilon)));
  EXPECT_TRUE((fuzzyCmpGreater<T, T, T>(0, -kFortyTwo)));

  EXPECT_TRUE((fuzzyCmpGreater<T, T, T>(kLargeNumber, kFortyTwo, kEpsilon)));
  EXPECT_TRUE((fuzzyCmpGreater<T, T, T>(kLargeNumber, kFortyTwo)));

  EXPECT_TRUE((fuzzyCmpGreater<T, T, T>(-kFortyTwo, -kLargeNumber, kEpsilon)));
  EXPECT_TRUE((fuzzyCmpGreater<T, T, T>(-kFortyTwo, -kLargeNumber)));
}

// fuzzyCmpGreaterEqual ----------------------------------------------------------------------------
TYPED_TEST(FloatingPointTest, FuzzyCmpGreaterEqualGivenEqualValues) {
  using T = TypeParam;

  static constexpr T kEpsilon = epsilon_v<T>;
  static constexpr T kFortyTwo = 42;
  static constexpr T kLargeNumber = 99'999;

  EXPECT_TRUE((fuzzyCmpGreaterEqual<T, T, T>(0, 0, kEpsilon)));
  EXPECT_TRUE((fuzzyCmpGreaterEqual<T, T, T>(0, 0)));

  EXPECT_TRUE((fuzzyCmpGreaterEqual<T, T, T>(-0, -0, kEpsilon)));
  EXPECT_TRUE((fuzzyCmpGreaterEqual<T, T, T>(-0, -0)));

  EXPECT_TRUE((fuzzyCmpGreaterEqual<T, T, T>(kFortyTwo, kFortyTwo, kEpsilon)));
  EXPECT_TRUE((fuzzyCmpGreaterEqual<T, T, T>(kFortyTwo, kFortyTwo)));

  EXPECT_TRUE((fuzzyCmpGreaterEqual<T, T, T>(-kFortyTwo, -kFortyTwo, kEpsilon)));
  EXPECT_TRUE((fuzzyCmpGreaterEqual<T, T, T>(-kFortyTwo, -kFortyTwo)));

  EXPECT_TRUE((fuzzyCmpGreaterEqual<T, T, T>(kLargeNumber, kLargeNumber, kEpsilon)));
  EXPECT_TRUE((fuzzyCmpGreaterEqual<T, T, T>(kLargeNumber, kLargeNumber)));

  EXPECT_TRUE((fuzzyCmpGreaterEqual<T, T, T>(-kLargeNumber, -kLargeNumber, kEpsilon)));
  EXPECT_TRUE((fuzzyCmpGreaterEqual<T, T, T>(-kLargeNumber, -kLargeNumber)));
}

TYPED_TEST(FloatingPointTest, FuzzyCmpGreaterEqualGivenTheFirstValueLessThanTheSecond) {
  using T = TypeParam;

  static constexpr T kEpsilon = epsilon_v<T>;
  static constexpr T kFortyTwo = 42;
  static constexpr T kLargeNumber = 99'999;

  EXPECT_FALSE((fuzzyCmpGreaterEqual<T, T, T>(0, kFortyTwo, kEpsilon)));
  EXPECT_FALSE((fuzzyCmpGreaterEqual<T, T, T>(0, kFortyTwo)));

  EXPECT_FALSE((fuzzyCmpGreaterEqual<T, T, T>(-kFortyTwo, 0, kEpsilon)));
  EXPECT_FALSE((fuzzyCmpGreaterEqual<T, T, T>(-kFortyTwo, 0)));

  EXPECT_FALSE((fuzzyCmpGreaterEqual<T, T, T>(kFortyTwo, kLargeNumber, kEpsilon)));
  EXPECT_FALSE((fuzzyCmpGreaterEqual<T, T, T>(kFortyTwo, kLargeNumber)));

  EXPECT_FALSE((fuzzyCmpGreaterEqual<T, T, T>(-kLargeNumber, -kFortyTwo, kEpsilon)));
  EXPECT_FALSE((fuzzyCmpGreaterEqual<T, T, T>(-kLargeNumber, -kFortyTwo)));
}

TYPED_TEST(FloatingPointTest, FuzzyCmpGreaterEqualGivenTheFirstValueGreaterThanTheSecond) {
  using T = TypeParam;

  static constexpr T kEpsilon = epsilon_v<T>;
  static constexpr T kFortyTwo = 42;
  static constexpr T kLargeNumber = 99'999;

  EXPECT_TRUE((fuzzyCmpGreaterEqual<T, T, T>(kFortyTwo, 0, kEpsilon)));
  EXPECT_TRUE((fuzzyCmpGreaterEqual<T, T, T>(kFortyTwo, 0)));

  EXPECT_TRUE((fuzzyCmpGreaterEqual<T, T, T>(0, -kFortyTwo, kEpsilon)));
  EXPECT_TRUE((fuzzyCmpGreaterEqual<T, T, T>(0, -kFortyTwo)));

  EXPECT_TRUE((fuzzyCmpGreaterEqual<T, T, T>(kLargeNumber, kFortyTwo, kEpsilon)));
  EXPECT_TRUE((fuzzyCmpGreaterEqual<T, T, T>(kLargeNumber, kFortyTwo)));

  EXPECT_TRUE((fuzzyCmpGreaterEqual<T, T, T>(-kFortyTwo, -kLargeNumber, kEpsilon)));
  EXPECT_TRUE((fuzzyCmpGreaterEqual<T, T, T>(-kFortyTwo, -kLargeNumber)));
}

// Functors ----------------------------------------------------------------------------------------
TYPED_TEST(FloatingPointTest, FuzzyIsZeroFunctorGivenZeros) {
  using T = TypeParam;

  static constexpr T kEpsilon = epsilon_v<T>;

  const FuzzyIsZero<T> kIsZero(kEpsilon);
  const FuzzyIsZero<T> kDefaultIsZero;

  EXPECT_TRUE(kIsZero(0));
  EXPECT_TRUE(kIsZero(-0));

  EXPECT_TRUE(kDefaultIsZero(0));
  EXPECT_TRUE(kDefaultIsZero(-0));
}

TYPED_TEST(FloatingPointTest, FuzzyEqualToFunctorGivenZeros) {
  using T = TypeParam;

  static constexpr T kEpsilon = epsilon_v<T>;

  const FuzzyEqualTo<T> kEqualTo(kEpsilon);
  const FuzzyEqualTo<T> kDefaultEqualTo;

  EXPECT_TRUE(kEqualTo(0, 0));
  EXPECT_TRUE(kEqualTo(-0, -0));

  EXPECT_TRUE(kDefaultEqualTo(0, 0));
  EXPECT_TRUE(kDefaultEqualTo(-0, -0));
}

TYPED_TEST(FloatingPointTest, FuzzyNotEqualToFunctorGivenZeros) {
  using T = TypeParam;

  static constexpr T kEpsilon = epsilon_v<T>;

  const FuzzyNotEqualTo<T> kNotEqualTo(kEpsilon);
  const FuzzyNotEqualTo<T> kDefaultNotEqualTo;

  EXPECT_FALSE(kNotEqualTo(0, 0));
  EXPECT_FALSE(kNotEqualTo(-0, -0));

  EXPECT_FALSE(kDefaultNotEqualTo(0, 0));
  EXPECT_FALSE(kDefaultNotEqualTo(-0, -0));
}

TYPED_TEST(FloatingPointTest, FuzzyThreeWayFunctorGivenZeros) {
  using T = TypeParam;

  static constexpr T kEpsilon = epsilon_v<T>;

  const FuzzyThreeWay<T> kThreeWay(kEpsilon);
  const FuzzyThreeWay<T> kDefaultThreeWay;

  EXPECT_TRUE(std::is_eq(kThreeWay(0, 0)));
  EXPECT_TRUE(std::is_eq(kThreeWay(-0, -0)));

  EXPECT_TRUE(std::is_eq(kDefaultThreeWay(0, 0)));
  EXPECT_TRUE(std::is_eq(kDefaultThreeWay(-0, -0)));
}

TYPED_TEST(FloatingPointTest, FuzzyLessFunctorGivenZeros) {
  using T = TypeParam;

  static constexpr T kEpsilon = epsilon_v<T>;

  const FuzzyLess<T> kLess(kEpsilon);
  const FuzzyLess<T> kDefaultLess;

  EXPECT_FALSE(kLess(0, 0));
  EXPECT_FALSE(kLess(-0, -0));

  EXPECT_FALSE(kDefaultLess(0, 0));
  EXPECT_FALSE(kDefaultLess(-0, -0));
}

TYPED_TEST(FloatingPointTest, FuzzyLessEqualFunctorGivenZero) {
  using T = TypeParam;

  static constexpr T kEpsilon = epsilon_v<T>;

  const FuzzyLessEqual<T> kLessEqual(kEpsilon);
  const FuzzyLessEqual<T> kDefaultLessEqual;

  EXPECT_TRUE(kLessEqual(0, 0));
  EXPECT_TRUE(kLessEqual(-0, -0));

  EXPECT_TRUE(kDefaultLessEqual(0, 0));
  EXPECT_TRUE(kDefaultLessEqual(-0, -0));
}

TYPED_TEST(FloatingPointTest, FuzzyGreaterFunctorGivenZeros) {
  using T = TypeParam;

  static constexpr T kEpsilon = epsilon_v<T>;

  const FuzzyGreater<T> kGreater(kEpsilon);
  const FuzzyGreater<T> kDefaultGreater;

  EXPECT_FALSE(kGreater(0, 0));
  EXPECT_FALSE(kGreater(-0, -0));

  EXPECT_FALSE(kDefaultGreater(0, 0));
  EXPECT_FALSE(kDefaultGreater(-0, -0));
}

TYPED_TEST(FloatingPointTest, FuzzyGreaterEqualFunctorGivenZeros) {
  using T = TypeParam;

  static constexpr T kEpsilon = epsilon_v<T>;

  const FuzzyGreaterEqual<T> kGreaterEqual(kEpsilon);
  const FuzzyGreaterEqual<T> kDefaultGreaterEqual;

  EXPECT_TRUE(kGreaterEqual(0, 0));
  EXPECT_TRUE(kGreaterEqual(-0, -0));

  EXPECT_TRUE(kDefaultGreaterEqual(0, 0));
  EXPECT_TRUE(kDefaultGreaterEqual(-0, -0));
}

} // namespace robocin

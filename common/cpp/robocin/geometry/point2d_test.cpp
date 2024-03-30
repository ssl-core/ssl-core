#include "robocin/geometry/point2d.h"
#include "robocin/utility/internal/test/epsilon_injector.h"

#include <gtest/gtest.h>

namespace robocin {
namespace {

using ::testing::Test;
using ::testing::Types;

template <class T>
class Point2DTest : public Test {};

using TestTypes = Types<std::int16_t, std::int32_t, std::int64_t, float, double, long double>;
TYPED_TEST_SUITE(Point2DTest, TestTypes);

// Static constructors -----------------------------------------------------------------------------

TYPED_TEST(Point2DTest, Origin) {
  const Point2D<TypeParam> kPt = Point2D<TypeParam>::origin();

  EXPECT_EQ(kPt.x, 0);
  EXPECT_EQ(kPt.y, 0);
}

TYPED_TEST(Point2DTest, FromPolarGivenAngle) {
  if constexpr (std::is_floating_point_v<TypeParam>) {
    static constexpr TypeParam kEpsilon = epsilon_v<TypeParam>;
    static constexpr TypeParam kPi = std::numbers::pi_v<TypeParam>;
    static constexpr TypeParam kSqrt2 = std::numbers::sqrt2_v<TypeParam>;

    struct InOut {
      TypeParam angle;
      Point2D<TypeParam> expected;
    };

    static constexpr std::array<InOut, 8> kOctants = {
        InOut{0.0, {/*x=*/1.0, /*y=*/0.0}},                                 // 0º
        InOut{kPi / 4.0, {/*x=*/kSqrt2 / 2.0, /*y=*/kSqrt2 / 2.0}},         // 45º
        InOut{kPi / 2.0, {/*x=*/0.0, /*y=*/1.0}},                           // 90º
        InOut{3.0 * kPi / 4.0, {/*x=*/-kSqrt2 / 2.0, /*y=*/kSqrt2 / 2.0}},  // 135º
        InOut{kPi, {/*x=*/-1.0, /*y=*/0.0}},                                // 180º
        InOut{5.0 * kPi / 4.0, {/*x=*/-kSqrt2 / 2.0, /*y=*/-kSqrt2 / 2.0}}, // 225º
        InOut{3.0 * kPi / 2.0, {/*x=*/0.0, /*y=*/-1.0}},                    // 270º
        InOut{7.0 * kPi / 4.0, {/*x=*/kSqrt2 / 2.0, /*y=*/-kSqrt2 / 2.0}},  // 315º
    };

    for (auto [angle, expected] : kOctants) {
      const Point2D<TypeParam> kPt = Point2D<TypeParam>::fromPolar(angle);

      EXPECT_NEAR(kPt.x, expected.x, kEpsilon);
      EXPECT_NEAR(kPt.y, expected.y, kEpsilon);
    }
  }
}

TYPED_TEST(Point2DTest, FromPolarGivenAngleAndLength) {
  if constexpr (std::is_floating_point_v<TypeParam>) {
    static constexpr TypeParam kEpsilon = epsilon_v<TypeParam>;
    static constexpr TypeParam kPi = std::numbers::pi_v<TypeParam>;
    static constexpr TypeParam kSqrt2 = std::numbers::sqrt2_v<TypeParam>;

    struct InOut {
      TypeParam angle;
      TypeParam length;
      Point2D<TypeParam> expected;
    };

    static constexpr std::array<InOut, 8> kOctants = {
        InOut{0.0, 1.0, {/*x=*/1.0, /*y=*/0.0}},                                  // 0º
        InOut{kPi / 4.0, 2.0, {/*x=*/kSqrt2 / 2.0, /*y=*/kSqrt2 / 2.0}},          // 45º
        InOut{kPi / 2.0, 4.0, {/*x=*/0.0, /*y=*/1.0}},                            // 90º
        InOut{3.0 * kPi / 4.0, 8.0, {/*x=*/-kSqrt2 / 2.0, /*y=*/kSqrt2 / 2.0}},   // 135º
        InOut{kPi, 16.0, {/*x=*/-1.0, /*y=*/0.0}},                                // 180º
        InOut{5.0 * kPi / 4.0, 32.0, {/*x=*/-kSqrt2 / 2.0, /*y=*/-kSqrt2 / 2.0}}, // 225º
        InOut{3.0 * kPi / 2.0, 64.0, {/*x=*/0.0, /*y=*/-1.0}},                    // 270º
        InOut{7.0 * kPi / 4.0, 128.0, {/*x=*/kSqrt2 / 2.0, /*y=*/-kSqrt2 / 2.0}}, // 315º
    };

    for (auto [angle, length, expected] : kOctants) {
      const Point2D<TypeParam> kPt = Point2D<TypeParam>::fromPolar(angle, length);

      EXPECT_NEAR(kPt.x, length * expected.x, kEpsilon);
      EXPECT_NEAR(kPt.y, length * expected.y, kEpsilon);
    }
  }
}

// Constructors ------------------------------------------------------------------------------------

TYPED_TEST(Point2DTest, DefaultConstructor) {
  const Point2D<TypeParam> kPt;

  EXPECT_EQ(kPt.x, 0);
  EXPECT_EQ(kPt.y, 0);
}

TYPED_TEST(Point2DTest, CopyConstructor) {
  const Point2D<TypeParam> kOther(/*x=*/1, /*y=*/2);
  const Point2D<TypeParam> kPt(kOther);

  EXPECT_EQ(kPt.x, kOther.x);
  EXPECT_EQ(kPt.y, kOther.y);
}

TYPED_TEST(Point2DTest, MoveConstructor) {
  Point2D<TypeParam> other(/*x=*/1, /*y=*/2);
  const Point2D<TypeParam> kPt(std::move(other));

  EXPECT_EQ(kPt.x, 1);
  EXPECT_EQ(kPt.y, 2);
}

TYPED_TEST(Point2DTest, ConstructorGivenXAndY) {
  const Point2D<TypeParam> kPt(/*x=*/1, /*y=*/2);

  EXPECT_EQ(kPt.x, 1);
  EXPECT_EQ(kPt.y, 2);
}

TYPED_TEST(Point2DTest, ConstructorGivenStructPoint) {
  struct OtherPoint2D {
    TypeParam x;
    TypeParam y;
  };

  const OtherPoint2D kOther{.x = 1, .y = 2};
  const Point2D<TypeParam> kPt(kOther);

  EXPECT_EQ(kPt.x, kOther.x);
  EXPECT_EQ(kPt.y, kOther.y);
}

TYPED_TEST(Point2DTest, ConstructorGivenClassPoint) {
  class OtherPoint2D {
   public:
    OtherPoint2D(TypeParam x, TypeParam y) : // NOLINT(bugprone-easily-swappable-parameters)
        x_(x),
        y_(y) {}

    [[nodiscard]] TypeParam x() const { return x_; }
    [[nodiscard]] TypeParam y() const { return y_; }

   private:
    TypeParam x_;
    TypeParam y_;
  };

  const OtherPoint2D kOther{/*x=*/1, /*y=*/2};
  const Point2D<TypeParam> kPt(kOther);

  EXPECT_EQ(kPt.x, kOther.x());
  EXPECT_EQ(kPt.y, kOther.y());
}

TYPED_TEST(Point2DTest, ConstructorGivenPair) {
  const std::pair kPair{1, 2};
  const Point2D<TypeParam> kPt(kPair);

  EXPECT_EQ(kPt.x, kPair.first);
  EXPECT_EQ(kPt.y, kPair.second);
}

// Validators --------------------------------------------------------------------------------------

TYPED_TEST(Point2DTest, IsNull) {
  const Point2D<TypeParam> kPt(/*x=*/0, /*y=*/0);
  EXPECT_TRUE(kPt.isNull());

  const Point2D<TypeParam> kPt2(/*x=*/1, /*y=*/2);
  EXPECT_FALSE(kPt2.isNull());
}

// Arithmetic assignment operators -----------------------------------------------------------------

TYPED_TEST(Point2DTest, AdditionAssignment) {
  Point2D<TypeParam> pt(/*x=*/1, /*y=*/2);
  pt += Point2D<TypeParam>(/*x=*/3, /*y=*/4);

  EXPECT_EQ(pt.x, 4);
  EXPECT_EQ(pt.y, 6);
}

TYPED_TEST(Point2DTest, SubtractionAssignment) {
  Point2D<TypeParam> pt(/*x=*/1, /*y=*/2);
  pt -= Point2D<TypeParam>(/*x=*/3, /*y=*/4);

  EXPECT_EQ(pt.x, -2);
  EXPECT_EQ(pt.y, -2);
}

TYPED_TEST(Point2DTest, MultiplicationAssignment) {
  Point2D<TypeParam> pt(/*x=*/1, /*y=*/2);
  pt *= 3;

  EXPECT_EQ(pt.x, 3);
  EXPECT_EQ(pt.y, 6);
}

TYPED_TEST(Point2DTest, DivisionAssignment) {
  Point2D<TypeParam> pt(/*x=*/1, /*y=*/2);
  pt /= 2;

  EXPECT_EQ(pt.x, static_cast<TypeParam>(1) / 2);
  EXPECT_EQ(pt.y, static_cast<TypeParam>(2) / 2);
}

// Arithmetic operators ----------------------------------------------------------------------------

TYPED_TEST(Point2DTest, Addition) {
  const Point2D<TypeParam> kPt1(/*x=*/1, /*y=*/2);
  const Point2D<TypeParam> kPt2(/*x=*/3, /*y=*/4);

  const Point2D<TypeParam> kPt3 = kPt1 + kPt2;

  EXPECT_EQ(kPt3.x, 4);
  EXPECT_EQ(kPt3.y, 6);
}

TYPED_TEST(Point2DTest, Subtraction) {
  const Point2D<TypeParam> kPt1(/*x=*/1, /*y=*/2);
  const Point2D<TypeParam> kPt2(/*x=*/3, /*y=*/4);

  const Point2D<TypeParam> kPt3 = kPt2 - kPt1;

  EXPECT_EQ(kPt3.x, 2);
  EXPECT_EQ(kPt3.y, 2);
}

TYPED_TEST(Point2DTest, Multiplication) {
  const Point2D<TypeParam> kPt1(/*x=*/1, /*y=*/2);
  const Point2D<TypeParam> kPt2 = kPt1 * 3;

  EXPECT_EQ(kPt2.x, 3);
  EXPECT_EQ(kPt2.y, 6);

  const Point2D<TypeParam> kPt3 = 3 * kPt1;

  EXPECT_EQ(kPt3.x, 3);
  EXPECT_EQ(kPt3.y, 6);
}

TYPED_TEST(Point2DTest, Division) {
  const Point2D<TypeParam> kPt1(/*x=*/1, /*y=*/2);
  const Point2D<TypeParam> kPt2 = kPt1 / 2;

  EXPECT_EQ(kPt2.x, static_cast<TypeParam>(1) / 2);
  EXPECT_EQ(kPt2.y, static_cast<TypeParam>(2) / 2);
}

// Sign operators ----------------------------------------------------------------------------------

TYPED_TEST(Point2DTest, UnaryPlus) {
  const Point2D<TypeParam> kPt1(/*x=*/1, /*y=*/2);
  const Point2D<TypeParam> kPt2 = +kPt1;

  EXPECT_EQ(kPt2.x, 1);
  EXPECT_EQ(kPt2.y, 2);
}

TYPED_TEST(Point2DTest, UnaryMinus) {
  const Point2D<TypeParam> kPt1(/*x=*/1, /*y=*/2);
  const Point2D<TypeParam> kPt2 = -kPt1;

  EXPECT_EQ(kPt2.x, -1);
  EXPECT_EQ(kPt2.y, -2);
}

// Equality operators ------------------------------------------------------------------------------

TYPED_TEST(Point2DTest, Equality) {
  const Point2D<TypeParam> kPt1(/*x=*/1, /*y=*/2);
  const Point2D<TypeParam> kPt2(/*x=*/1, /*y=*/2);

  EXPECT_TRUE(kPt1 == kPt2);

  const Point2D<TypeParam> kPt3(/*x=*/3, /*y=*/4);
  EXPECT_FALSE(kPt1 == kPt3);
}

// Three-way comparison operator -------------------------------------------------------------------

TYPED_TEST(Point2DTest, ThreeWayComparison) {
  const Point2D<TypeParam> kPt1(/*x=*/1, /*y=*/2);
  const Point2D<TypeParam> kPt2(/*x=*/1, /*y=*/2);

  EXPECT_EQ(kPt1 <=> kPt2, std::strong_ordering::equal);

  const Point2D<TypeParam> kPt3(/*x=*/3, /*y=*/4);
  EXPECT_EQ(kPt1 <=> kPt3, std::strong_ordering::less);
  EXPECT_EQ(kPt3 <=> kPt1, std::strong_ordering::greater);
}

// Swap --------------------------------------------------------------------------------------------

TYPED_TEST(Point2DTest, Swapping) {
  Point2D<TypeParam> pt1(/*x=*/1, /*y=*/2);
  Point2D<TypeParam> pt2(/*x=*/3, /*y=*/4);

  pt1.swap(pt2);

  EXPECT_EQ(pt1.x, 3);
  EXPECT_EQ(pt1.y, 4);
  EXPECT_EQ(pt2.x, 1);
  EXPECT_EQ(pt2.y, 2);
}

// Geometry ----------------------------------------------------------------------------------------

TYPED_TEST(Point2DTest, DotProduct) {
  const Point2D<TypeParam> kPt1(/*x=*/1, /*y=*/2);
  const Point2D<TypeParam> kPt2(/*x=*/3, /*y=*/4);

  EXPECT_EQ(kPt1.dot(kPt2), 11);
}

TYPED_TEST(Point2DTest, CrossProduct) {
  const Point2D<TypeParam> kPt1(/*x=*/1, /*y=*/2);
  const Point2D<TypeParam> kPt2(/*x=*/3, /*y=*/4);

  EXPECT_EQ(kPt1.cross(kPt2), -2);
}

TYPED_TEST(Point2DTest, ManhattanLength) {
  const Point2D<TypeParam> kPt1(/*x=*/1, /*y=*/2);

  EXPECT_EQ(kPt1.manhattanLength(), 3);
}

TYPED_TEST(Point2DTest, ManhattanDistTo) {
  const Point2D<TypeParam> kPt1(/*x=*/1, /*y=*/2);
  const Point2D<TypeParam> kPt2(/*x=*/3, /*y=*/4);

  EXPECT_EQ(kPt1.manhattanDistTo(kPt2), 4);
}

TYPED_TEST(Point2DTest, LengthSquared) {
  const Point2D<TypeParam> kPt1(/*x=*/1, /*y=*/2);

  EXPECT_EQ(kPt1.lengthSquared(), 5);
}

TYPED_TEST(Point2DTest, Length) {
  const Point2D<TypeParam> kPt1(/*x=*/1, /*y=*/2);

  using point2d_t = Point2D<TypeParam>;
  using length_result_t = std::invoke_result_t<decltype(&point2d_t::length), const point2d_t&>;

  static constexpr std::floating_point auto kEpsilon = epsilon_v<length_result_t>;

  EXPECT_NEAR(kPt1.length(), std::sqrt(5), kEpsilon);
}

TYPED_TEST(Point2DTest, Norm) {
  const Point2D<TypeParam> kPt1(/*x=*/1, /*y=*/2);

  using point2d_t = Point2D<TypeParam>;
  using norm_result_t = std::invoke_result_t<decltype(&point2d_t::norm), const point2d_t&>;

  static constexpr std::floating_point auto kEpsilon = epsilon_v<norm_result_t>;

  EXPECT_NEAR(kPt1.norm(), std::sqrt(5), kEpsilon);
}

TYPED_TEST(Point2DTest, DistSquaredTo) {
  const Point2D<TypeParam> kPt1(/*x=*/1, /*y=*/2);
  const Point2D<TypeParam> kPt2(/*x=*/3, /*y=*/4);

  EXPECT_EQ(kPt1.distSquaredTo(kPt2), 8);
}

TYPED_TEST(Point2DTest, DistTo) {
  const Point2D<TypeParam> kPt1(/*x=*/1, /*y=*/2);
  const Point2D<TypeParam> kPt2(/*x=*/3, /*y=*/4);

  using point2d_t = Point2D<TypeParam>;
  using dist_to_result_t = std::invoke_result_t<decltype(&point2d_t::distTo), //
                                                const point2d_t&,             //
                                                const point2d_t&>;

  static constexpr std::floating_point auto kEpsilon = epsilon_v<dist_to_result_t>;

  EXPECT_NEAR(kPt1.distTo(kPt2), std::sqrt(8), kEpsilon);
}

TYPED_TEST(Point2DTest, Angle) {
  const Point2D<TypeParam> kPt1(/*x=*/0, /*y=*/1);

  using point2d_t = Point2D<TypeParam>;
  using angle_result_t = std::invoke_result_t<decltype(&point2d_t::angle), const point2d_t&>;

  static constexpr angle_result_t kEpsilon = epsilon_v<angle_result_t>;
  static constexpr std::floating_point auto kPi = std::numbers::pi_v<angle_result_t>;

  EXPECT_NEAR(kPt1.angle(), kPi / 2, kEpsilon);
}

TYPED_TEST(Point2DTest, AngleTo) {
  const Point2D<TypeParam> kPt1(/*x=*/0, /*y=*/1);
  const Point2D<TypeParam> kPt2(/*x=*/1, /*y=*/0);

  using point2d_t = Point2D<TypeParam>;
  using angle_to_result_t = std::invoke_result_t<decltype(&point2d_t::angleTo), //
                                                 const point2d_t&,              //
                                                 const point2d_t&>;

  static constexpr angle_to_result_t kEpsilon = epsilon_v<angle_to_result_t>;
  static constexpr std::floating_point auto kPi = std::numbers::pi_v<angle_to_result_t>;

  EXPECT_NEAR(kPt1.angleTo(kPt2), -kPi / 2, kEpsilon);
}

// Rotations ---------------------------------------------------------------------------------------

TYPED_TEST(Point2DTest, RotateCW90) {
  Point2D<TypeParam> pt1(/*x=*/1, /*y=*/2);
  pt1.rotateCW90();

  EXPECT_EQ(pt1.x, 2);
  EXPECT_EQ(pt1.y, -1);
}

TYPED_TEST(Point2DTest, RotatedCW90) {
  const Point2D<TypeParam> kPt1(/*x=*/1, /*y=*/2);
  const Point2D<TypeParam> kPt2 = kPt1.rotatedCW90();

  EXPECT_EQ(kPt2.x, 2);
  EXPECT_EQ(kPt2.y, -1);
}

TYPED_TEST(Point2DTest, RotateCCW90) {
  Point2D<TypeParam> pt1(/*x=*/1, /*y=*/2);
  pt1.rotateCCW90();

  EXPECT_EQ(pt1.x, -2);
  EXPECT_EQ(pt1.y, 1);
}

TYPED_TEST(Point2DTest, RotatedCCW90) {
  const Point2D<TypeParam> kPt1(/*x=*/1, /*y=*/2);
  const Point2D<TypeParam> kPt2 = kPt1.rotatedCCW90();

  EXPECT_EQ(kPt2.x, -2);
  EXPECT_EQ(kPt2.y, 1);
}

TYPED_TEST(Point2DTest, RotateCW) {
  if constexpr (std::floating_point<TypeParam>) {
    static constexpr std::floating_point auto kEpsilon = epsilon_v<TypeParam>;
    static constexpr std::floating_point auto kPi = std::numbers::pi_v<TypeParam>;

    Point2D<TypeParam> pt1(/*x=*/1, /*y=*/2);
    pt1.rotateCW(kPi / 2);

    EXPECT_NEAR(pt1.x, 2, kEpsilon);
    EXPECT_NEAR(pt1.y, -1, kEpsilon);
  }
}

TYPED_TEST(Point2DTest, RotatedCW) {
  if constexpr (std::floating_point<TypeParam>) {
    static constexpr std::floating_point auto kEpsilon = epsilon_v<TypeParam>;
    static constexpr std::floating_point auto kPi = std::numbers::pi_v<TypeParam>;

    const Point2D<TypeParam> kPt1(/*x=*/1, /*y=*/2);
    const Point2D<TypeParam> kPt2 = kPt1.rotatedCW(kPi / 2);

    EXPECT_NEAR(kPt2.x, 2, kEpsilon);
    EXPECT_NEAR(kPt2.y, -1, kEpsilon);
  }
}

TYPED_TEST(Point2DTest, RotateCCW) {
  if constexpr (std::floating_point<TypeParam>) {
    static constexpr std::floating_point auto kEpsilon = epsilon_v<TypeParam>;
    static constexpr std::floating_point auto kPi = std::numbers::pi_v<TypeParam>;

    Point2D<TypeParam> pt1(/*x=*/1, /*y=*/2);
    pt1.rotateCCW(kPi / 2);

    EXPECT_NEAR(pt1.x, -2, kEpsilon);
    EXPECT_NEAR(pt1.y, 1, kEpsilon);
  }
}

TYPED_TEST(Point2DTest, RotatedCCW) {
  if constexpr (std::floating_point<TypeParam>) {
    static constexpr std::floating_point auto kEpsilon = epsilon_v<TypeParam>;
    static constexpr std::floating_point auto kPi = std::numbers::pi_v<TypeParam>;

    const Point2D<TypeParam> kPt1(/*x=*/1, /*y=*/2);
    const Point2D<TypeParam> kPt2 = kPt1.rotatedCCW(kPi / 2);

    EXPECT_NEAR(kPt2.x, -2, kEpsilon);
    EXPECT_NEAR(kPt2.y, 1, kEpsilon);
  }
}

// Resizing and Normalization ----------------------------------------------------------------------

TYPED_TEST(Point2DTest, Resize) {
  if constexpr (std::floating_point<TypeParam>) {
    static constexpr TypeParam kResizeFactor = 2;
    static constexpr std::floating_point auto kEpsilon = epsilon_v<TypeParam>;

    Point2D<TypeParam> pt1(/*x=*/1, /*y=*/2);
    pt1.resize(kResizeFactor);

    EXPECT_NEAR(pt1.x, 1.0 * kResizeFactor / std::sqrt(5), kEpsilon);
    EXPECT_NEAR(pt1.y, 2.0 * kResizeFactor / std::sqrt(5), kEpsilon);
  }
}

TYPED_TEST(Point2DTest, Resized) {
  if constexpr (std::floating_point<TypeParam>) {
    static constexpr TypeParam kResizeFactor = 2;
    static constexpr std::floating_point auto kEpsilon = epsilon_v<TypeParam>;

    const Point2D<TypeParam> kPt1(/*x=*/1, /*y=*/2);
    const Point2D<TypeParam> kPt2 = kPt1.resized(kResizeFactor);

    EXPECT_NEAR(kPt2.x, 1.0 * kResizeFactor / std::sqrt(5), kEpsilon);
    EXPECT_NEAR(kPt2.y, 2.0 * kResizeFactor / std::sqrt(5), kEpsilon);
  }
}

TYPED_TEST(Point2DTest, NormalizeFloatingPoint) {
  if constexpr (std::floating_point<TypeParam>) {
    static constexpr std::floating_point auto kEpsilon = epsilon_v<TypeParam>;

    Point2D<TypeParam> pt1(/*x=*/1, /*y=*/2);
    pt1.normalize();

    EXPECT_NEAR(pt1.x, 1.0 / std::sqrt(5), kEpsilon);
    EXPECT_NEAR(pt1.y, 2.0 / std::sqrt(5), kEpsilon);
  }
}

TYPED_TEST(Point2DTest, NormalizeIntegral) {
  if constexpr (std::integral<TypeParam>) {
    Point2D<TypeParam> pt1(/*x=*/1, /*y=*/2);
    pt1.normalize();

    EXPECT_EQ(pt1.x, 1);
    EXPECT_EQ(pt1.y, 2);
  }
}

TYPED_TEST(Point2DTest, NormalizedFloatingPoint) {
  if constexpr (std::floating_point<TypeParam>) {
    static constexpr std::floating_point auto kEpsilon = epsilon_v<TypeParam>;

    const Point2D<TypeParam> kPt1(/*x=*/1, /*y=*/2);
    const Point2D<TypeParam> kPt2 = kPt1.normalized();

    EXPECT_NEAR(kPt2.x, 1.0 / std::sqrt(5), kEpsilon);
    EXPECT_NEAR(kPt2.y, 2.0 / std::sqrt(5), kEpsilon);
  }
}

TYPED_TEST(Point2DTest, NormalizedIntegral) {
  if constexpr (std::integral<TypeParam>) {
    const Point2D<TypeParam> kPt1(/*x=*/1, /*y=*/2);
    const Point2D<TypeParam> kPt2 = kPt1.normalized();

    EXPECT_EQ(kPt2.x, 1);
    EXPECT_EQ(kPt2.y, 2);
  }
}

TYPED_TEST(Point2DTest, AxesNormalize) {
  Point2D<TypeParam> pt1(/*x=*/1, /*y=*/2);
  pt1.axesNormalize();

  EXPECT_EQ(pt1.x, 1);
  EXPECT_EQ(pt1.y, 1);
}

TYPED_TEST(Point2DTest, AxesNormalized) {
  const Point2D<TypeParam> kPt1(/*x=*/1, /*y=*/2);
  const Point2D<TypeParam> kPt2 = kPt1.axesNormalized();

  EXPECT_EQ(kPt2.x, 1);
  EXPECT_EQ(kPt2.y, 1);
}

// Array-like --------------------------------------------------------------------------------------

TYPED_TEST(Point2DTest, Size) {
  static_assert(Point2D<TypeParam>::size() == 2);

  const Point2D<TypeParam> kPt;
  EXPECT_EQ(kPt.size(), 2);
}

TYPED_TEST(Point2DTest, DimensionalAccess) {
  Point2D<TypeParam> pt(/*x=*/1, /*y=*/2);

  EXPECT_EQ(pt[0], 1);
  EXPECT_EQ(pt[1], 2);

  EXPECT_EQ(&(pt[0]), &(pt.x));
  EXPECT_EQ(&(pt[1]), &(pt.y));

  EXPECT_THROW(pt[2], std::out_of_range);

  const Point2D<TypeParam> kPt(/*x=*/3, /*y=*/4);

  EXPECT_EQ(kPt[0], 3);
  EXPECT_EQ(kPt[1], 4);

  EXPECT_THROW(kPt[2], std::out_of_range);

  static_assert(!std::is_reference_v<decltype(kPt[0])>); // const point returns a copy.
  static_assert(!std::is_reference_v<decltype(kPt[1])>); // const point returns a copy.
}

// Iterators ---------------------------------------------------------------------------------------

TYPED_TEST(Point2DTest, Iterator) {
  EXPECT_TRUE(std::random_access_iterator<typename Point2D<TypeParam>::iterator>);
  EXPECT_TRUE(std::random_access_iterator<typename Point2D<TypeParam>::const_iterator>);
  EXPECT_TRUE(std::random_access_iterator<typename Point2D<TypeParam>::reverse_iterator>);
  EXPECT_TRUE(std::random_access_iterator<typename Point2D<TypeParam>::const_reverse_iterator>);
}

TYPED_TEST(Point2DTest, Begin) {
  Point2D<TypeParam> pt(/*x=*/1, /*y=*/2);
  EXPECT_EQ(&(*pt.begin()), &(pt.x));

  const Point2D<TypeParam> kPt(/*x=*/3, /*y=*/4);
  EXPECT_EQ(&(*kPt.begin()), &(kPt.x));

  static_assert(std::is_same_v<decltype(kPt.begin()), typename Point2D<TypeParam>::const_iterator>);
}

TYPED_TEST(Point2DTest, End) {
  Point2D<TypeParam> pt(/*x=*/1, /*y=*/2);
  EXPECT_THROW(*pt.end(), std::out_of_range);

  const Point2D<TypeParam> kPt(/*x=*/3, /*y=*/4);
  EXPECT_THROW(*kPt.end(), std::out_of_range);

  static_assert(std::is_same_v<decltype(kPt.end()), typename Point2D<TypeParam>::const_iterator>);
}

TYPED_TEST(Point2DTest, Cbegin) {
  Point2D<TypeParam> pt(/*x=*/1, /*y=*/2); // NOLINT(misc-const-correctness)
  EXPECT_EQ(&(*pt.cbegin()), &(pt.x));

  const Point2D<TypeParam> kPt(/*x=*/3, /*y=*/4);
  EXPECT_EQ(&(*kPt.cbegin()), &(kPt.x));
}

TYPED_TEST(Point2DTest, Cend) {
  Point2D<TypeParam> pt(/*x=*/1, /*y=*/2); // NOLINT(misc-const-correctness)
  EXPECT_THROW(*pt.cend(), std::out_of_range);

  const Point2D<TypeParam> kPt(/*x=*/3, /*y=*/4);
  EXPECT_THROW(*kPt.cend(), std::out_of_range);
}

TYPED_TEST(Point2DTest, Rbegin) {
  Point2D<TypeParam> pt(/*x=*/1, /*y=*/2);
  EXPECT_EQ(&(*pt.rbegin()), &(pt.y));

  const Point2D<TypeParam> kPt(/*x=*/3, /*y=*/4);
  EXPECT_EQ(&(*kPt.rbegin()), &(kPt.y));

  static_assert(
      std::is_same_v<decltype(kPt.rbegin()), typename Point2D<TypeParam>::const_reverse_iterator>);
}

TYPED_TEST(Point2DTest, Rend) {
  Point2D<TypeParam> pt(/*x=*/1, /*y=*/2);
  EXPECT_THROW(*pt.rend(), std::out_of_range);

  const Point2D<TypeParam> kPt(/*x=*/3, /*y=*/4);
  EXPECT_THROW(*kPt.rend(), std::out_of_range);

  static_assert(
      std::is_same_v<decltype(kPt.rend()), typename Point2D<TypeParam>::const_reverse_iterator>);
}

TYPED_TEST(Point2DTest, Crbegin) {
  Point2D<TypeParam> pt(/*x=*/1, /*y=*/2); // NOLINT(misc-const-correctness)
  EXPECT_EQ(&(*pt.crbegin()), &(pt.y));

  const Point2D<TypeParam> kPt(/*x=*/3, /*y=*/4);
  EXPECT_EQ(&(*kPt.crbegin()), &(kPt.y));
}

TYPED_TEST(Point2DTest, Crend) {
  Point2D<TypeParam> pt(/*x=*/1, /*y=*/2); // NOLINT(misc-const-correctness)
  EXPECT_THROW(*pt.crend(), std::out_of_range);

  const Point2D<TypeParam> kPt(/*x=*/3, /*y=*/4);
  EXPECT_THROW(*kPt.crend(), std::out_of_range);
}

// Input/Output ------------------------------------------------------------------------------------

TYPED_TEST(Point2DTest, Input) {
  Point2D<TypeParam> pt;
  std::istringstream iss("1 2");
  iss >> pt;

  EXPECT_EQ(pt.x, 1);
  EXPECT_EQ(pt.y, 2);
}

TYPED_TEST(Point2DTest, Output) {
  const Point2D<TypeParam> kPt(/*x=*/1, /*y=*/2);
  std::ostringstream oss;
  oss << kPt;

  EXPECT_EQ(oss.str(), "(x = 1, y = 2)");
}

// Deduction Guides --------------------------------------------------------------------------------

TEST(Point2DTest, EmptyDeductionGuide) {
  const Point2D kPt;

  EXPECT_EQ(kPt.x, 0);
  EXPECT_EQ(kPt.y, 0);

  static_assert(std::is_same_v<decltype(kPt)::value_type, double>);
}

TEST(Point2DTest, ValueDeductionGuide) {
  static constexpr int kFirstCoord = 1;
  static constexpr double kSecondCoord = 2.0;

  const Point2D kPt(kFirstCoord, kSecondCoord);

  using common_type_t = std::common_type_t<decltype(kFirstCoord), decltype(kSecondCoord)>;

  EXPECT_EQ(kPt.x, static_cast<common_type_t>(kFirstCoord));
  EXPECT_EQ(kPt.y, static_cast<common_type_t>(kSecondCoord));

  static_assert(std::is_same_v<common_type_t, double>);
}

} // namespace
} // namespace robocin

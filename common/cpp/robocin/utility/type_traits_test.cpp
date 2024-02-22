#include "robocin/utility/type_traits.h"

#include <concepts>
#include <gtest/gtest.h>

namespace robocin {

TEST(CommonTypeForComparison, GivenTwoIntsReturnsDouble) {
  using expected_t = common_floating_point_for_comparison_t<int, int>;

  EXPECT_TRUE((std::same_as<expected_t, double>) );
}

TEST(CommonTypeForComparison, GivenIntAndFloatReturnsFloat) {
  using expected_t = common_floating_point_for_comparison_t<int, float>;

  EXPECT_TRUE((std::same_as<expected_t, float>) );
}

TEST(CommonTypeForComparison, GivenIntAndDoubleReturnsDouble) {
  using expected_t = common_floating_point_for_comparison_t<int, double>;

  EXPECT_TRUE((std::same_as<expected_t, double>) );
}

TEST(CommonTypeForComparison, GivenFloatAndDoubleReturnsFloat) {
  using expected_t = common_floating_point_for_comparison_t<float, double>;

  EXPECT_TRUE((std::same_as<expected_t, float>) );
}

TEST(CommonTypeForComparison, GivenIntAndFloatAndDoubleReturnsFloat) {
  using expected_t = common_floating_point_for_comparison_t<int, float, double>;

  EXPECT_TRUE((std::same_as<expected_t, float>) );
}

} // namespace robocin

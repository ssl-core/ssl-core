#include "robocin/utility/type_traits.h"

#include <concepts>
#include <gtest/gtest.h>

namespace robocin {

TEST(CommonTypeForComparison, GivenTwoIntsReturnsDouble) {
  using Result = common_floating_point_for_comparison_t<int, int>;

  EXPECT_TRUE((std::same_as<Result, double>) );
}

TEST(CommonTypeForComparison, GivenIntAndFloatReturnsFloat) {
  using Result = common_floating_point_for_comparison_t<int, float>;

  EXPECT_TRUE((std::same_as<Result, float>) );
}

TEST(CommonTypeForComparison, GivenIntAndDoubleReturnsDouble) {
  using Result = common_floating_point_for_comparison_t<int, double>;

  EXPECT_TRUE((std::same_as<Result, double>) );
}

TEST(CommonTypeForComparison, GivenFloatAndDoubleReturnsFloat) {
  using Result = common_floating_point_for_comparison_t<float, double>;

  EXPECT_TRUE((std::same_as<Result, float>) );
}

TEST(CommonTypeForComparison, GivenIntAndFloatAndDoubleReturnsFloat) {
  using Result = common_floating_point_for_comparison_t<int, float, double>;

  EXPECT_TRUE((std::same_as<Result, float>) );
}

} // namespace robocin

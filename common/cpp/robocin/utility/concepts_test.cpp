#include "robocin/utility/concepts.h"

#include <gtest/gtest.h>

namespace robocin {

using ::testing::Test;
using ::testing::Types;

using ArithmeticTestTypes = Types<bool,      //
                                  char,      //
                                  char8_t,   //
                                  char16_t,  //
                                  char32_t,  //
                                  wchar_t,   //
                                  short,     // NOLINT(*int*)
                                  int,       //
                                  long,      // NOLINT(*int*)
                                  long long, // NOLINT(*int*)
                                  float,     //
                                  double,    //
                                  long double>;

template <class>
class ArithmeticTest : public Test {};
TYPED_TEST_SUITE(ArithmeticTest, ArithmeticTestTypes);

// protobufish -------------------------------------------------------------------------------------
TEST(ProtobufishTest, IsProtobufish) {
  struct Protobufish : ::google::protobuf::MessageLite {};

  EXPECT_TRUE(protobufish<Protobufish&>);
  EXPECT_TRUE(protobufish<const Protobufish&>);
  EXPECT_TRUE(protobufish<Protobufish&&>);
  EXPECT_TRUE(protobufish<const Protobufish&&>);
  EXPECT_TRUE(protobufish<volatile Protobufish&>);
  EXPECT_TRUE(protobufish<const volatile Protobufish&>);
  EXPECT_TRUE(protobufish<Protobufish volatile&&>);
  EXPECT_TRUE(protobufish<const volatile Protobufish&&>);
}

TEST(ProtobufishTest, IsNotProtobufish) {
  struct NotProtobufish {};

  EXPECT_FALSE(protobufish<NotProtobufish&>);
  EXPECT_FALSE(protobufish<const NotProtobufish&>);
  EXPECT_FALSE(protobufish<NotProtobufish&&>);
  EXPECT_FALSE(protobufish<const NotProtobufish&&>);
  EXPECT_FALSE(protobufish<volatile NotProtobufish&>);
  EXPECT_FALSE(protobufish<const volatile NotProtobufish&>);
  EXPECT_FALSE(protobufish<NotProtobufish volatile&&>);
  EXPECT_FALSE(protobufish<const volatile NotProtobufish&&>);
}

// arithmetic --------------------------------------------------------------------------------------
TYPED_TEST(ArithmeticTest, IsArithmetic) {
  using T = TypeParam;

  EXPECT_TRUE(arithmetic<T>);
  EXPECT_TRUE(arithmetic<T&>);
  EXPECT_TRUE(arithmetic<const T&>);
  EXPECT_TRUE(arithmetic<T&&>);
  EXPECT_TRUE(arithmetic<const T&&>);
  EXPECT_TRUE(arithmetic<volatile T>);
  EXPECT_TRUE(arithmetic<const volatile T>);
  EXPECT_TRUE(arithmetic<volatile T&&>);
  EXPECT_TRUE(arithmetic<const volatile T&&>);
}

TEST(ArithmeticTest, IsNotArithmetic) {
  struct NotArithmetic {};

  EXPECT_FALSE(arithmetic<NotArithmetic&>);
  EXPECT_FALSE(arithmetic<const NotArithmetic&>);
  EXPECT_FALSE(arithmetic<NotArithmetic&&>);
  EXPECT_FALSE(arithmetic<const NotArithmetic&&>);
  EXPECT_FALSE(arithmetic<volatile NotArithmetic&>);
  EXPECT_FALSE(arithmetic<const volatile NotArithmetic&>);
  EXPECT_FALSE(arithmetic<volatile NotArithmetic&&>);
  EXPECT_FALSE(arithmetic<const volatile NotArithmetic&&>);
}

} // namespace robocin

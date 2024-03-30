#include "robocin/utility/concepts.h"

#include <cstddef>
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

// fundamentalish ----------------------------------------------------------------------------------

using FundamentalishTestTypes = Types<bool,  //
                                      char,  //
                                      int,   //
                                      float, //
                                      std::nullptr_t>;

template <class>
class FundamentalishTest : public Test {};
TYPED_TEST_SUITE(FundamentalishTest, FundamentalishTestTypes);

TYPED_TEST(FundamentalishTest, IsFundamentalish) {
  using T = TypeParam;

  EXPECT_TRUE(fundamentalish<T>);
}

TEST(FundamentalishTest, IsFundamentalishWhenVoid) { EXPECT_TRUE(fundamentalish<void>); }

TYPED_TEST(FundamentalishTest, IsNotFundamentalishWhenRefQualified) {
  using T = TypeParam;

  EXPECT_FALSE(fundamentalish<T&>);
  EXPECT_FALSE(fundamentalish<const T&>);
  EXPECT_FALSE(fundamentalish<T&&>);
  EXPECT_FALSE(fundamentalish<const T&&>);
}

TYPED_TEST(FundamentalishTest, IsNotFundamentalishWhenPointerQualified) {
  using T = TypeParam;

  EXPECT_FALSE(fundamentalish<T*>);
  EXPECT_FALSE(fundamentalish<const T*>);
}

TEST(FundamentalishTest, IsNotFundamentalishWhenNotFundamental) {
  struct NotFundamental {};

  EXPECT_FALSE(fundamentalish<NotFundamental>);
}

// stringish ---------------------------------------------------------------------------------------

TEST(StringishTest, IsStringishWhenStdString) {
  EXPECT_TRUE(stringish<std::string>);
  EXPECT_TRUE(stringish<std::wstring>);
  EXPECT_TRUE(stringish<std::u8string>);
  EXPECT_TRUE(stringish<std::u16string>);
  EXPECT_TRUE(stringish<std::u32string>);
}

TEST(StringishTest, IsStringishWhenStdStringView) {
  EXPECT_TRUE(stringish<std::string_view>);
  EXPECT_TRUE(stringish<std::wstring_view>);
  EXPECT_TRUE(stringish<std::u8string_view>);
  EXPECT_TRUE(stringish<std::u16string_view>);
  EXPECT_TRUE(stringish<std::u32string_view>);
}

TEST(StringishTest, IsStringishWhenCharPtr) {
  EXPECT_TRUE(stringish<char*>);
  EXPECT_TRUE(stringish<const char*>);
  EXPECT_TRUE(stringish<wchar_t*>);
  EXPECT_TRUE(stringish<const wchar_t*>);
  EXPECT_TRUE(stringish<char8_t*>);
  EXPECT_TRUE(stringish<const char8_t*>);
  EXPECT_TRUE(stringish<char16_t*>);
  EXPECT_TRUE(stringish<const char16_t*>);
  EXPECT_TRUE(stringish<char32_t*>);
  EXPECT_TRUE(stringish<const char32_t*>);
}

TEST(StringishTest, IsNotStringishWhenNotStringish) {
  struct NotStringish {};

  EXPECT_FALSE(stringish<NotStringish>);
}

// optionalish -------------------------------------------------------------------------------------

TEST(OptionalishTest, IsOptionalishWhenStdOptional) {
  struct Value {};

  EXPECT_TRUE(optionalish<std::optional<Value>&>);
  EXPECT_TRUE(optionalish<const std::optional<Value>&>);
  EXPECT_TRUE(optionalish<std::optional<Value>&&>);
  EXPECT_TRUE(optionalish<const std::optional<Value>&&>);
  EXPECT_TRUE(optionalish<volatile std::optional<Value>&>);
  EXPECT_TRUE(optionalish<const volatile std::optional<Value>&>);
  EXPECT_TRUE(optionalish<std::optional<Value> volatile&&>);
  EXPECT_TRUE(optionalish<const volatile std::optional<Value>&&>);
}

TEST(OptionalishTest, IsNotOptionalishWhenNotOptionalish) {
  struct NotOptionalish {};

  EXPECT_FALSE(optionalish<NotOptionalish>);
}

} // namespace robocin

#include "robocin/macros/builder.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace robocin {

using ::testing::ElementsAre;
using ::testing::Pair;

struct Value {
  friend bool operator==(const Value& lhs, const Value& rhs) = default;
};

class NonDefault {
 public:
  explicit NonDefault(int x) : x_(x) {}

 private:
  int x_;
};

class Buildable final {
  BuildableWith(Builder);

  Getter(Default, Checked, bool);
  Getter(Default, Fundamental, int);
  Getter(Default, String, std::string);
  Getter(Optional, OptionalValue, Value);
  Getter(Optional, Val, NonDefault);
  Getter(Default, DictOfNumberNames, std::map<int, std::string>);
  Getter(Repeated, VectorOfNumbers, std::vector<int>);
};

class Buildable::Builder final {
  BuilderOf(Buildable);

  Setter(Checked);
  Setter(Fundamental);
  Setter(String);
  Setter(OptionalValue);
  Setter(DictOfNumberNames);
  Setter(VectorOfNumbers);
};

TEST(BuilderTest, BuildBuildableClass) {
  const Buildable buildable = Buildable::Builder{}
                                  .setChecked(true)
                                  .setFundamental(1)
                                  .setString("robocin")
                                  .setOptionalValue(Value{})
                                  .setDictOfNumberNames({{1, "one"}, {2, "two"}})
                                  .setVectorOfNumbers({1, 2})
                                  .build();

  EXPECT_EQ(buildable.getFundamental(), 1);
  EXPECT_TRUE((std::same_as<decltype(buildable.getFundamental()), int>) );

  EXPECT_EQ(buildable.getString(), "robocin");
  EXPECT_TRUE((std::same_as<decltype(buildable.getString()), std::string_view>) );

  EXPECT_EQ(buildable.getOptionalValue(), Value{});
  EXPECT_TRUE((std::same_as<decltype(buildable.getOptionalValue()), const Value&>) );

  EXPECT_EQ(buildable.getOptionalOptionalValue(), std::optional{Value{}});
  EXPECT_TRUE(
      (std::same_as<decltype(buildable.getOptionalOptionalValue()), const std::optional<Value>&>) );

  EXPECT_THAT(buildable.getDictOfNumberNames(), ElementsAre(Pair(1, "one"), Pair(2, "two")));
  EXPECT_TRUE((std::same_as<decltype(buildable.getDictOfNumberNames()),
                            const std::map<int, std::string>&>) );

  EXPECT_THAT(buildable.getVectorOfNumbers(), ElementsAre(1, 2));
  EXPECT_TRUE((std::same_as<decltype(buildable.getVectorOfNumbers()), Span<const int>>) );

  EXPECT_TRUE(buildable.isChecked());
  EXPECT_TRUE((std::same_as<decltype(buildable.isChecked()), bool>) );
}

} // namespace robocin

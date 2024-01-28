#include "robocin/utility/concepts.h"

#include <gtest/gtest.h>

namespace robocin {

TEST(ConceptsTest, IsProtobufish) {
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

TEST(ConceptsTest, IsNotProtobufish) {
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

} // namespace robocin

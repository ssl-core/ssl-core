#include "robocin/utility/concepts.h"

#include <gtest/gtest.h>

namespace robocin {
namespace {

TEST(ConceptsTest, IsProtobufish) {
  struct Protobufish : ::google::protobuf::MessageLite {};

  EXPECT_TRUE(protobufish<Protobufish&> &&                //
              protobufish<const Protobufish&> &&          //
              protobufish<Protobufish&&> &&               //
              protobufish<const Protobufish&&> &&         //
              protobufish<volatile Protobufish&> &&       //
              protobufish<const volatile Protobufish&> && //
              protobufish<Protobufish volatile&&> &&      //
              protobufish<const volatile Protobufish&&>);
}

TEST(ConceptsTest, IsNotProtobufish) {
  struct NotProtobufish {};

  EXPECT_FALSE(protobufish<NotProtobufish&> ||                //
               protobufish<const NotProtobufish&> ||          //
               protobufish<NotProtobufish&&> ||               //
               protobufish<const NotProtobufish&&> ||         //
               protobufish<volatile NotProtobufish&> ||       //
               protobufish<const volatile NotProtobufish&> || //
               protobufish<NotProtobufish volatile&&> ||      //
               protobufish<const volatile NotProtobufish&&>);
}

} // namespace
} // namespace robocin

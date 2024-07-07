// Original Source:
// https://github.com/protocolbuffers/protobuf/blob/main/src/google/protobuf/test_textproto.h

// Protocol Buffers - Google's data interchange format
// Copyright 2023 Google Inc.  All rights reserved.
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file or at
// https://developers.google.com/open-source/licenses/bsd

#ifndef ROBOCIN_THIRD_PARTY_GOOGLE_PROTOBUF_TEST_TEXTPROTO_H
#define ROBOCIN_THIRD_PARTY_GOOGLE_PROTOBUF_TEST_TEXTPROTO_H

#include <absl/log/absl_check.h>
#include <absl/memory/memory.h>
#include <gmock/gmock.h>
#include <google/protobuf/dynamic_message.h>
#include <google/protobuf/text_format.h>

// This file contains private helpers for dealing with textprotos in our
// tests.  We make no guarantees about the behavior in real-world situations,
// and these are only meant for basic unit-tests of protobuf internals.

namespace google::protobuf {

MATCHER_P(EqualsProto, textproto, "") { // NOLINT(*const*)
  auto msg = absl::WrapUnique(arg.New());
  return TextFormat::ParseFromString(textproto, msg.get())
         && msg->DebugString() == arg.DebugString();
}

MATCHER_P3(EqualsProtoSerialized, pool, type, textproto, "") { // NOLINT(*const*)
  const Descriptor* desc = pool->FindMessageTypeByName(type);
  DynamicMessageFactory factory(pool);
  auto msg = absl::WrapUnique(factory.GetPrototype(desc)->New());
  return TextFormat::ParseFromString(textproto, msg.get())
         && arg.SerializeAsString() == msg->SerializeAsString();
}

class ParseTextOrDie {
 public:
  explicit ParseTextOrDie(std::string_view text) : text_(text) {}

  template <class Proto>
  operator Proto() { // NOLINT(*explicit*)
    Proto ret;
    ABSL_CHECK(TextFormat::ParseFromString(std::string{text_}, &ret));
    return ret;
  }

 private:
  std::string_view text_;
};

} // namespace google::protobuf

#endif // ROBOCIN_THIRD_PARTY_GOOGLE_PROTOBUF_TEST_TEXTPROTO_H

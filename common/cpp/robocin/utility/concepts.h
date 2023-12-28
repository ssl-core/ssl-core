#ifndef ROBOCIN_UTILITY_CONCEPTS_H
#define ROBOCIN_UTILITY_CONCEPTS_H

#include <concepts>
#include <google/protobuf/message_lite.h>

namespace robocin {

template <class T>
concept protobuf = std::derived_from<std::decay_t<T>, ::google::protobuf::MessageLite>;

} // namespace robocin

#endif // ROBOCIN_UTILITY_CONCEPTS_H

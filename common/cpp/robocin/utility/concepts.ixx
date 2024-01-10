module;

#include <concepts>
#include <google/protobuf/message_lite.h>

export module robocin.utility:concepts;

export namespace robocin {

template <class T>
concept protobufish = std::derived_from<std::remove_cvref_t<T>, ::google::protobuf::MessageLite>;

} // namespace robocin

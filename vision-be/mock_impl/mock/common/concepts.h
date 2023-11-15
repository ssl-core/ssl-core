#ifndef INES_COMMON_CONCEPTS_H
#define INES_COMMON_CONCEPTS_H

#include <concepts>
#include <type_traits>

#include <google/protobuf/message_lite.h>

namespace ines {

template <class T>
concept protobuf = std::derived_from<std::decay_t<T>, google::protobuf::MessageLite>;

} // namespace ines

#endif // INES_COMMON_CONCEPTS_H

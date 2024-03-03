#ifndef ROBOCIN_CONTAINER_SPAN_H
#define ROBOCIN_CONTAINER_SPAN_H

#include <span>

namespace robocin {

template <class T, std::size_t Extent = std::dynamic_extent>
using Span = std::span<T, Extent>;

} // namespace robocin

#endif // ROBOCIN_CONTAINER_SPAN_H

#ifndef ROBOCIN_CONTAINER_SPAN_H
#define ROBOCIN_CONTAINER_SPAN_H

#include <array>
#include <vector>

namespace robocin {

template <class T, class Container = std::vector<T>>
using Collection = Container;

template <class T, std::size_t N>
using ArrayCollection = Collection<T, std::array<T, N>>;

} // namespace robocin

#endif // ROBOCIN_CONTAINER_SPAN_H

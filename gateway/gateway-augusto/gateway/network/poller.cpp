#include "gateway/network/poller.h"

namespace gateway {

Poller::Poller(std::span<const int> file_descriptors) {
  pollfds_.reserve(file_descriptors.size());

  for (int fd : file_descriptors) {
    pollfds_.push_back({
        .fd = fd,
        .events = POLLIN,
    });
  }
}

void Poller::push(int file_descriptor) {
  pollfds_.push_back({
      .fd = file_descriptor,
      .events = POLLIN,
  });
}

void Poller::poll(int timeout) { ::poll(pollfds_.data(), pollfds_.size(), timeout); }

} // namespace gateway

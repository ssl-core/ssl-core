#include "vision/network/poller.h"

namespace vision {

void Poller::add(std::unique_ptr<ISocketReceiver>& socket) {
  sockets_[socket.get()] = static_cast<int>(fds_.size()); //?????? cast really needed ?????
  fds_.push_back({
      .fd = socket->fileDescriptor(),
      .events = POLLIN,
  });
}

void Poller::poll(int timeout) { ::poll(fds_.data(), fds_.size(), timeout); }

std::string Poller::recvFrom(std::unique_ptr<ISocketReceiver>& socket) {
  if (fds_[sockets_[socket.get()]].revents & POLLIN) { // NOLINT
    return socket->receive();
  }

  return {};
}

void Poller::close() {
  for (const auto& socket : sockets_) {
    socket.first->close();
  }
}
} // namespace vision

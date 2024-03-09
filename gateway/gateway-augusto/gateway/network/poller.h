#ifndef GATEWAY_NETWORK_POLLER_H
#define GATEWAY_NETWORK_POLLER_H

#include <memory>
#include <span>
#include <sys/poll.h>
#include <vector>

namespace gateway {

class Poller {
 public:
  Poller() = default;
  explicit Poller(std::span<const int> file_descriptors);

  void push(int file_descriptor);
  void poll(int timeout);

  template <class Socket>
  Socket::receive_type recvFrom(Socket& socket) const {
    for (const ::pollfd& pollfd : pollfds_) {
      if (pollfd.fd == socket.fd()) {
        if (pollfd.revents & POLLIN) { // NOLINT(*bitwise*)
          return socket.receive();
        }
      }
    }
    return {};
  }

 private:
  std::vector<::pollfd> pollfds_;
};

} // namespace gateway

#endif // GATEWAY_NETWORK_POLLER_H

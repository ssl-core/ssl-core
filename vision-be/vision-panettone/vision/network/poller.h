#ifndef VISION_NETWORK_POLLER_H
#define VISION_NETWORK_POLLER_H

#include "socket.h"

#include <memory>
#include <string>
#include <sys/poll.h>
#include <unordered_map>
#include <vector>

namespace vision {

class Poller {
 public:
  Poller() = default;

  void add(std::unique_ptr<ISocketReceiver>& socket);

  void poll(int timeout);

  std::string recvFrom(std::unique_ptr<ISocketReceiver>& socket);

  // void remove(std::unique_ptr<ISocketReceiver>& socket);

  void close();

 private:
  // TODO(aalmds): improve mapping to optimize remove complexity.
  std::unordered_map<ISocketReceiver*, int> sockets_;
  std::vector<pollfd> fds_;
};
} // namespace vision

#endif // VISION_NETWORK_POLLER_H

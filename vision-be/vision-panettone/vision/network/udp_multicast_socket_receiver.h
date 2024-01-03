

#ifndef VISION_NETWORK_UPD_MULTICAST_SOCKET_RECEIVER_H
#define VISION_NETWORK_UPD_MULTICAST_SOCKET_RECEIVER_H

#include "vision/network/socket.h"

#include <string>

namespace vision {

class UdpMulticastSocketReceiver : public ISocketReceiver {
 public:
  UdpMulticastSocketReceiver(const std::string& ip_address,
                             const std::string& inet_address,
                             int port);

  std::string receive() override;

  void close() override;

  [[nodiscard]] int fileDescriptor() const override;

 private:
  int socket_;
};

} // namespace vision

#endif // VISION_NETWORK_UPD_MULTICAST_SOCKET_RECEIVER_H

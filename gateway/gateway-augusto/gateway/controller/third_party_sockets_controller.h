#ifndef GATEWAY_CONTROLLER_THIRD_PARTY_SOCKETS_CONTROLLER
#define GATEWAY_CONTROLLER_THIRD_PARTY_SOCKETS_CONTROLLER

#include "gateway/controller/icontroller.h"
#include "gateway/network/poller.h"
#include "robocin/network/udp_multicast_socket_receiver.h"
#include "robocin/network/zmq_publisher_socket.h"

#include <arpa/inet.h>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

namespace gateway {

class ThirdPartySocketsController : public IController {
 public:
  explicit ThirdPartySocketsController();
  void run() override;

  static bool isSocketConnected(int sockfd) {
    int error = 0;
    socklen_t len = sizeof(error);
    int retval = getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, &len);

    if (retval != 0) {
      // Error occurred while getting socket options
      std::cerr << "Error occurred while getting socket options" << std::endl;
      return false;
    }

    if (error != 0) {
      // Socket is not connected
      std::cerr << "Socket is not connected: " << strerror(error) << std::endl;
      return false;
    }

    // Socket is connected
    return true;
  }

 private:
  robocin::UdpMulticastSocketReceiver vision_;
  robocin::UdpMulticastSocketReceiver referee_;
  robocin::UdpMulticastSocketReceiver tracked_;

  robocin::ZmqPublisherSocket publisher_;

  Poller poller_;
};

} // namespace gateway

#endif // GATEWAY_CONTROLLER_THIRD_PARTY_SOCKETS_CONTROLLER

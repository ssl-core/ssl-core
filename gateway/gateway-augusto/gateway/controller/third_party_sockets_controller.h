#ifndef GATEWAY_CONTROLLER_THIRD_PARTY_SOCKETS_CONTROLLER
#define GATEWAY_CONTROLLER_THIRD_PARTY_SOCKETS_CONTROLLER

#include "gateway/controller/icontroller.h"
#include "gateway/network/poller.h"
#include "robocin/network/udp_multicast_socket_receiver.h"
#include "robocin/network/zmq_publisher_socket.h"

namespace gateway {

class ThirdPartySocketsController : public IController {
 public:
  explicit ThirdPartySocketsController();
  void run() override;

 private:
  robocin::UdpMulticastSocketReceiver vision_;
  robocin::UdpMulticastSocketReceiver referee_;
  robocin::UdpMulticastSocketReceiver tracked_;

  robocin::ZmqPublisherSocket publisher_;

  Poller poller_;
};

} // namespace gateway

#endif // GATEWAY_CONTROLLER_THIRD_PARTY_SOCKETS_CONTROLLER

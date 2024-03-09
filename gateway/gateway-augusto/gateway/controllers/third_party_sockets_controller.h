#ifndef GATEWAY_CONTROLLERS_THIRD_PARTY_SOCKETS_CONTROLLER
#define GATEWAY_CONTROLLERS_THIRD_PARTY_SOCKETS_CONTROLLER

#include "gateway/network/poller.h"
#include "robocin/network/udp_multicast_socket_receiver.h"
#include "robocin/network/zmq_publisher_socket.h"
#include "gateway/controllers/controller.h"

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

#endif // GATEWAY_CONTROLLERS_THIRD_PARTY_SOCKETS_CONTROLLER

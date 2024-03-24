#ifndef GATEWAY_CONTROLLER_SYNCHRONOUS_SOCKETS_CONTROLLER_H
#define GATEWAY_CONTROLLER_SYNCHRONOUS_SOCKETS_CONTROLLER_H

#include "gateway/controller/icontroller.h"
#include "gateway/network/zmq_router_dealer_socket.h"

namespace gateway {

class SynchronousSocketsController : public IController {
 public:
  explicit SynchronousSocketsController();
  void run() override;

 private:
  ZmqRouterSocket router_;
  ZmqDealerSocket dealer_;
};

} // namespace gateway

#endif // GATEWAY_CONTROLLER_SYNCHRONOUS_SOCKETS_CONTROLLER_H

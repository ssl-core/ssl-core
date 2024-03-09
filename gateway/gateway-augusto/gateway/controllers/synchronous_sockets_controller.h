#ifndef GATEWAY_CONTROLLERS_SYNCHRONOUS_SOCKETS_CONTROLLER_H
#define GATEWAY_CONTROLLERS_SYNCHRONOUS_SOCKETS_CONTROLLER_H

#include "gateway/zmq_router_dealer_socket.h"
#include "gateway/controllers/controller.h"

namespace gateway {
using robocin::ZmqDealerSocket;
using robocin::ZmqRouterSocket;

class SynchronousSocketsController : public IController {
 public:
  explicit SynchronousSocketsController();
  void run() override;

 private:
  ZmqRouterSocket router_;
  ZmqDealerSocket dealer_;
};

} // namespace gateway

#endif // GATEWAY_CONTROLLERS_SYNCHRONOUS_SOCKETS_CONTROLLER_H

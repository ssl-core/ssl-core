#ifndef GATEWAY_CONTROLLER_GRPC_CONTROLLER_H
#define GATEWAY_CONTROLLER_GRPC_CONTROLLER_H

#include "gateway/controller/icontroller.h"

#include <grpcpp/server_builder.h>

namespace gateway {

class GrpcController : public IController {
 public:
  explicit GrpcController(std::string_view address);
  void run() override;

 private:
  std::string address_;
};

} // namespace gateway

#endif // GATEWAY_CONTROLLER_GRPC_CONTROLLER_H

#ifndef GATEWAY_CONTROLLERS_I_CONTROLLER
#define GATEWAY_CONTROLLERS_I_CONTROLLER

namespace gateway {

class IController {
 public:
  IController() = default;

  IController(const IController&) = delete;
  IController& operator=(const IController&) = delete;
  IController(IController&&) noexcept = delete;
  IController& operator=(IController&&) noexcept = delete;

  virtual ~IController() = default;

  virtual void run() = 0;
};

} // namespace gateway

#endif // GATEWAY_CONTROLLERS_I_CONTROLLER

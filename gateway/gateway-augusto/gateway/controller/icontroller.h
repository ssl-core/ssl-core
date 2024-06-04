#ifndef GATEWAY_CONTROLLER_ICONTROLLER
#define GATEWAY_CONTROLLER_ICONTROLLER

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

#endif // GATEWAY_CONTROLLER_ICONTROLLER

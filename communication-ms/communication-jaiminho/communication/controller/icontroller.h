#ifndef COMMUNICATION_CONTROLLER_ICONTROLLER
#define COMMUNICATION_CONTROLLER_ICONTROLLER

namespace communication {

class IController {
 public:
  IController() = default;

  IController(const IController&) = delete;
  IController& operator=(const IController&) = delete;
  IController(IController&&) noexcept = default;
  IController& operator=(IController&&) noexcept = default;

  virtual ~IController() = default;

  virtual void run() = 0;
};

} // namespace communication

#endif // COMMUNICATION_CONTROLLER_ICONTROLLER

#ifndef DECISION_CONTROLLER_ICONTROLLER
#define DECISION_CONTROLLER_ICONTROLLER

namespace decision {

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

} // namespace decision

#endif // DECISION_CONTROLLER_ICONTROLLER

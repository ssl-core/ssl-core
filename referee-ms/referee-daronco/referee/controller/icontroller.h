#ifndef REFEREE_CONTROLLER_ICONTROLLER
#define REFEREE_CONTROLLER_ICONTROLLER

namespace referee {

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

} // namespace referee

#endif // REFEREE_CONTROLLER_ICONTROLLER

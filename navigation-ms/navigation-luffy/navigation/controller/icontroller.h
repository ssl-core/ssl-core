#ifndef NAVIGATION_CONTROLLER_ICONTROLLER
#define NAVIGATION_CONTROLLER_ICONTROLLER

namespace navigation {

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

} // namespace navigation

#endif // NAVIGATION_CONTROLLER_ICONTROLLER

#ifndef BEHAVIOR_CONTROLLER_ICONTROLLER
#define BEHAVIOR_CONTROLLER_ICONTROLLER

namespace behavior {

/**
 * @class IController
 * @brief An interface for controller classes in the behavior module.
 *
 * The IController interface defines a standard structure for controller classes,
 * enforcing the implementation of the run method, which should contain the main
 * logic for the controller.
 */
class IController {
 public:
  IController() = default;

  IController(const IController&) = delete;
  IController& operator=(const IController&) = delete;
  IController(IController&&) noexcept = default;
  IController& operator=(IController&&) noexcept = default;

  virtual ~IController() = default;

  /**
   * @brief Pure virtual method to be implemented by derived classes.
   *
   * The run method should contain the main logic for the controller. It is intended
   * to be overridden by any class that inherits from IController.
   */
  virtual void run() = 0;
};

} // namespace behavior

#endif // BEHAVIOR_CONTROLLER_ICONTROLLER

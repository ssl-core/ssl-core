#ifndef PERCEPTION_CONTROLLER_ICONTROLLER
#define PERCEPTION_CONTROLLER_ICONTROLLER

namespace perception {

/**
 * @class IController
 * @brief An interface for controller classes in the perception module.
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
  IController(IController&&) noexcept = delete;
  IController& operator=(IController&&) noexcept = delete;

  virtual ~IController() = default;

  /**
   * @brief Pure virtual method to be implemented by derived classes.
   *
   * The run method should contain the main logic for the controller. It is intended
   * to be overridden by any class that inherits from IController.
   */
  virtual void run() = 0;
};

} // namespace perception

#endif // PERCEPTION_CONTROLLER_ICONTROLLER

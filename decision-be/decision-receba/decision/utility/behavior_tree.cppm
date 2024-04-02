module;

export module decision.utility:behavior_tree;

namespace robocin {

template <class T>
using observer_ptr = T*;

enum class Status {
  Unspecified,
  Success,
  Failure,
  Running,
};

class AbstractNode; // abstract class                // DONE
class TaskNode;     // inherits from AbstractNode // DONE
class ControlNode;  // inherits from AbstractNode
class SelectorNode; // inherits from ControlNode
class SequenceNode; // inherits from ControlNode

} // namespace robocin

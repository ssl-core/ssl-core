#ifndef ROBOCIN_BEHAVIOR_TREE_BEHAVIOR_TREE_H
#define ROBOCIN_BEHAVIOR_TREE_BEHAVIOR_TREE_H

#include <concepts>
#include <memory>
#include <span>
#include <string>
#include <vector>

namespace robocin {
namespace behavior_tree_internal {

std::string demangle_or_default(const char* mangled); // NOLINT(*naming*)

} // namespace behavior_tree_internal

template <class>
class BehaviorTree;

template <class R, class... Ts>
class BehaviorTree<R(Ts...)> {
 public:
  enum class Status {
    Unspecified,
    Success,
    Failure,
    Running, // TODO(joseviccruz): Consider remove Running status when using std::expected.
  };

  class AbstractNode;       // parent of all nodes
  class ControlNode;        // inherits from AbstractNode
  class SelectorNode;       // inherits from ControlNode
  class SequenceNode;       // inherits from ControlNode
  class TaskNode;           // inherits from AbstractNode
  class ValidationTaskNode; // inherits from TaskNode (used with selector nodes for validation-only)

  class DebugNode;

  // TODO(joseviccruz): Replace pair result_type with std::expected<Status, Result>.
  using result_type = std::conditional_t<std::is_void_v<R>, Status, std::pair<Status, R>>;

  BehaviorTree() = delete;

  BehaviorTree(const BehaviorTree&) = delete;
  BehaviorTree& operator=(const BehaviorTree&) = delete;

  BehaviorTree(BehaviorTree&&) = default;
  BehaviorTree& operator=(BehaviorTree&&) = default;

  ~BehaviorTree() = default;

  template <std::derived_from<AbstractNode> T>
  explicit BehaviorTree(std::unique_ptr<T> root) : root_(std::move(root)) {
    root_->build();
  }

  [[nodiscard]] result_type operator()(Ts... ts) const { return root_->next(ts...); }
  [[nodiscard]] std::unique_ptr<DebugNode> debug() const { return root_->debug(); }

 private:
  std::unique_ptr<AbstractNode> root_;

  template <class... Args>
  static result_type make_result(Status status, Args&&... args) { // NOLINT(*naming*)
    if constexpr (std::is_void_v<R>) {
      return Status{status};
    } else {
      return {Status{status}, R{std::forward<Args>(args)...}};
    }
  }

  static Status get_status(const result_type& result) { // NOLINT(*naming*)
    if constexpr (std::is_void_v<R>) {
      return result;
    } else {
      return result.first;
    }
  }
};

template <class R, class... Ts>
class BehaviorTree<R(Ts...)>::AbstractNode {
 public:
  using result_type = typename BehaviorTree<R(Ts...)>::result_type;

  AbstractNode() = default;

  AbstractNode(const AbstractNode&) = delete;
  AbstractNode& operator=(const AbstractNode&) = delete;

  AbstractNode(AbstractNode&&) = default;
  AbstractNode& operator=(AbstractNode&&) = default;

  virtual ~AbstractNode() = default;

  virtual void build() = 0;
  [[nodiscard]] virtual bool abort(Ts... ts) const = 0;
  [[nodiscard]] virtual result_type next(Ts... ts) const = 0;

  template <class Self>
  std::unique_ptr<DebugNode> debug(this const Self& self) {
    auto node = std::make_unique<DebugNode>(self);

    for (const auto& edge : self.edges()) {
      node->edges_.push_back(edge->debug());
    }
    return node;
  }

  [[nodiscard]] virtual std::string_view type() const { return "Abstract"; }

 protected:
  [[nodiscard]] virtual std::span<const std::unique_ptr<AbstractNode>> edges() const { return {}; }
};

template <class R, class... Ts>
class BehaviorTree<R(Ts...)>::ControlNode : public AbstractNode {
 public:
  [[nodiscard]] std::span<const std::unique_ptr<AbstractNode>> edges() const final {
    return edges_;
  }

  [[nodiscard]] std::string_view type() const override { return "Control"; }

 protected:
  void add(std::unique_ptr<AbstractNode> edge) {
    edge->build();
    edges_.push_back(std::move(edge));
  }

 private:
  std::vector<std::unique_ptr<AbstractNode>> edges_;
};

template <class R, class... Ts>
class BehaviorTree<R(Ts...)>::SequenceNode : public ControlNode {
 public:
  using Status = typename BehaviorTree<R(Ts...)>::Status;

  using result_type = typename BehaviorTree<R(Ts...)>::result_type;
  using ControlNode::abort;

  [[nodiscard]] result_type next(Ts... ts) const final {
    if (abort(ts...)) {
      return make_result(Status::Failure);
    }
    for (const auto& edge : edges()) {
      if (result_type result = edge->next(ts...); get_status(result) != Status::Success) {
        return result;
      }
    }
    return make_result(Status::Success);
  }

  [[nodiscard]] std::string_view type() const final { return "Sequence"; }

 protected:
  using ControlNode::edges;
};

template <class R, class... Ts>
class BehaviorTree<R(Ts...)>::SelectorNode : public ControlNode {
 public:
  using Status = typename BehaviorTree<R(Ts...)>::Status;

  using result_type = typename BehaviorTree<R(Ts...)>::result_type;
  using ControlNode::abort;

  [[nodiscard]] result_type next(Ts... ts) const final {
    if (abort(ts...)) {
      return make_result(Status::Failure);
    }
    for (const auto& edge : edges()) {
      if (result_type result = edge->next(ts...); get_status(result) != Status::Failure) {
        return result;
      }
    }
    return make_result(Status::Failure);
  }

  [[nodiscard]] std::string_view type() const final { return "Selector"; }

 protected:
  using ControlNode::edges;
};

template <class R, class... Ts>
class BehaviorTree<R(Ts...)>::TaskNode : public AbstractNode {
 public:
  using Status = typename BehaviorTree<R(Ts...)>::Status;

  using result_type = typename BehaviorTree<R(Ts...)>::result_type;
  using AbstractNode::abort;

  void build() override {}

  [[nodiscard]] result_type next(Ts... ts) const final {
    if (abort(ts...)) {
      return make_result(Status::Failure);
    }
    return run(ts...);
  }

  [[nodiscard]] virtual result_type run(Ts... ts) const = 0;

  [[nodiscard]] std::string_view type() const override { return "Task"; }
};

template <class R, class... Ts>
class BehaviorTree<R(Ts...)>::ValidationTaskNode : public TaskNode {
 public:
  using Status = typename BehaviorTree<R(Ts...)>::Status;

  using result_type = typename BehaviorTree<R(Ts...)>::result_type;

  [[nodiscard]] std::string_view type() const final { return "ValidationTask"; }

 private:
  [[nodiscard]] virtual result_type run([[maybe_unused]] Ts... ts) const final {
    make_result(Status::Success);
  }
};

template <class R, class... Ts>
class BehaviorTree<R(Ts...)>::DebugNode {
 public:
  template <class T>
  explicit DebugNode(const T& node) :
      // TODO(joseviccruz): use robocin::nameof<T>().
      name_{behavior_tree_internal::demangle_or_default(typeid(node).name())},
      type_{node.type()} {}

  [[nodiscard]] std::string_view name() const { return name_; }
  [[nodiscard]] std::string_view type() const { return type_; }
  [[nodiscard]] std::span<const std::unique_ptr<DebugNode>> edges() const { return edges_; }

 private:
  template <class>
  friend class BehaviorTree;

  std::string name_;
  std::string type_;
  std::vector<std::unique_ptr<DebugNode>> edges_;
};

} // namespace robocin

#endif // ROBOCIN_BEHAVIOR_TREE_BEHAVIOR_TREE_H

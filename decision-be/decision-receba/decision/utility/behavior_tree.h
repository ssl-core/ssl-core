#ifndef DECISION_UTILITY_BEHAVIOR_TREE_BEHAVIOR_TREE_H
#define DECISION_UTILITY_BEHAVIOR_TREE_BEHAVIOR_TREE_H

#include <concepts>
#include <cstddef>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <string_view>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <variant>

namespace robocin {

template <class T>
using observer_ptr = T*;

enum class task_status_t {
  Success,
  Failure,
  Running,
};

class abstract_node_t; // abstract class                // DONE
class task_node_t;     // inherits from abstract_node_t // DONE
class control_node_t;  // inherits from abstract_node_t
class selector_node_t; // inherits from control_node_t
class sequence_node_t; // inherits from control_node_t

class behavior_tree_t;

template <class T>
concept AbstractNode = std::derived_from<T, abstract_node_t>;

template <class T>
concept ControlNode = std::derived_from<T, control_node_t>;

class abstract_node_t {
 public:
  virtual ~abstract_node_t() = default;

  [[nodiscard]] control_node_t* parent() const {
    return parent_;
  }

  virtual abstract_node_t* get_next_node() = 0;

  virtual bool abort() {
    return false;
  }
  // virtual bool retry() { // limpa o contexto e tenta novamente
  //   return false;
  // }
  virtual void reset() {
  }

 private:
  friend task_node_t;
  friend control_node_t;
  friend selector_node_t;
  friend sequence_node_t;

  friend behavior_tree_t;

  void set_parent(control_node_t* parent) {
    parent_ = parent;
  }
  virtual void internal_reset() = 0;

  static abstract_node_t* get_next_node_after_success_and_reset(abstract_node_t* node);
  static abstract_node_t* get_next_node_after_failure_and_reset(abstract_node_t* node);
  static control_node_t* get_last_aborted_parent(control_node_t* node);
  static abstract_node_t* get_next_node_after_abort_and_reset_until(abstract_node_t* node,
                                                                    control_node_t* until);

  control_node_t* parent_{nullptr};
};

class control_node_t : public abstract_node_t {
 public:
  virtual void build() = 0;

  virtual abstract_node_t* get_next_node_after_success() = 0;
  virtual abstract_node_t* get_next_node_after_failure() = 0;
};

class sequence_node_t : public control_node_t {
  using edges_t = std::vector<std::unique_ptr<abstract_node_t>>;

 public:
  template <AbstractNode Node, class... Args>
  void emplace(Args&&... args) {
    std::unique_ptr<Node> node = std::make_unique<Node>(std::forward<Args>(args)...);
    if constexpr (ControlNode<Node>) {
      node->build();
    }
    node->set_parent(this);
    edges_.emplace_back(std::move(node));
  }

  abstract_node_t* get_next_node() final {
    if (control_node_t* aborted_parent = get_last_aborted_parent(parent())) {
      return get_next_node_after_abort_and_reset_until(this, aborted_parent);
    }
    if (abort()) {
      return get_next_node_after_failure_and_reset(this);
    }
    if (pos_ < edges_.size()) {
      return edges_.at(pos_)->get_next_node();
    }
    return get_next_node_after_success_and_reset(this);
  }

  abstract_node_t* get_next_node_after_success() final {
    ++pos_;
    return get_next_node();
  }

  abstract_node_t* get_next_node_after_failure() final {
    if (pos_ == 0) {
      return get_next_node_after_failure_and_reset(this);
    }
    --pos_;
    return get_next_node();
  }

 private:
  void internal_reset() final {
    pos_ = 0;
    reset();
  }

  std::size_t pos_{0};
  edges_t edges_;
};

class selector_node_t : public control_node_t {
  using bool_fn_t = std::function<bool()>;
  using edges_t = std::vector<std::pair<bool_fn_t, std::unique_ptr<abstract_node_t>>>;

 public:
  template <AbstractNode Node, class BoolFn, class... Args>
  void emplace(BoolFn&& condition, Args&&... args) {
    std::unique_ptr<Node> node = std::make_unique<Node>(std::forward<Args>(args)...);
    if constexpr (ControlNode<Node>) {
      node->build();
    }
    node->set_parent(this);
    if constexpr (std::is_member_function_pointer_v<BoolFn>) {
      edges_.emplace_back(
          [this, condition = std::forward<BoolFn>(condition)]() {
            return std::invoke(condition, this);
          },
          std::move(node));
    } else {
      edges_.emplace_back(std::forward<BoolFn>(condition), std::move(node));
    }
  }

  abstract_node_t* get_next_node() final {
    if (control_node_t* aborted_parent = get_last_aborted_parent(parent())) {
      return get_next_node_after_abort_and_reset_until(this, aborted_parent);
    }
    if (abort()) {
      return get_next_node_after_failure_and_reset(this);
    }
    for (; pos_ < edges_.size(); ++pos_) {
      if (auto&& condition = std::get<bool_fn_t>(edges_.at(pos_)); condition()) {
        using node_ptr_t = std::unique_ptr<abstract_node_t>;
        return std::get<node_ptr_t>(edges_.at(pos_))->get_next_node();
      }
    }
    return get_next_node_after_failure_and_reset(this);
  }

  abstract_node_t* get_next_node_after_success() final {
    return get_next_node_after_success_and_reset(this);
  }

  abstract_node_t* get_next_node_after_failure() final {
    ++pos_;
    return get_next_node();
  }

 private:
  void internal_reset() final {
    pos_ = 0;
    reset();
  }

  std::size_t pos_{0};
  edges_t edges_;
};

class task_node_t : public abstract_node_t {
 public:
  virtual task_status_t run() = 0;

  abstract_node_t* get_next_node() final {
    if (control_node_t* aborted_parent = get_last_aborted_parent(parent())) {
      return get_next_node_after_abort_and_reset_until(this, aborted_parent);
    }
    if (abort()) {
      return get_next_node_after_failure_and_reset(this);
    }
    task_status_t status = run();
    if (status == task_status_t::Success) {
      return get_next_node_after_success_and_reset(this);
    }
    if (status == task_status_t::Failure) {
      return get_next_node_after_failure_and_reset(this);
    }
    return this;
  }

 private:
  void internal_reset() final {
    reset();
  }
};

class behavior_tree_t {
 public:
  template <ControlNode Node, class... Args>
  static behavior_tree_t make(Args&&... args) {
    std::unique_ptr<Node> root = std::make_unique<Node>(std::forward<Args>(args)...);
    root->build();

    return behavior_tree_t(std::move(root));
  }

  void run() {
    current_ = current_->get_next_node();
    if (current_ == nullptr) {
      current_ = root_->get_next_node();
    }
  }

  void reset() {
    while (current_ != nullptr) {
      current_->internal_reset();
      current_ = current_->parent();
    }
    current_ = root_.get();
  }

 private:
  explicit behavior_tree_t(std::unique_ptr<abstract_node_t> root) :
      root_(std::move(root)),
      current_(root_.get()) {
  }

  std::unique_ptr<abstract_node_t> root_;
  abstract_node_t* current_{nullptr};
};

} // namespace robocin

#endif // DECISION_UTILITY_BEHAVIOR_TREE_BEHAVIOR_TREE_H

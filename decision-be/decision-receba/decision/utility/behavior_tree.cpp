#include "decision/utility/behavior_tree.h"

namespace robocin {

abstract_node_t* abstract_node_t::get_next_node_after_success_and_reset(abstract_node_t* node) {
  node->internal_reset();
  return node->parent() != nullptr ? node->parent()->get_next_node_after_success() : nullptr;
}

abstract_node_t* abstract_node_t::get_next_node_after_failure_and_reset(abstract_node_t* node) {
  node->internal_reset();
  return node->parent() != nullptr ? node->parent()->get_next_node_after_failure() : nullptr;
}

control_node_t* abstract_node_t::get_last_aborted_parent(control_node_t* node) {
  control_node_t* aborted_parent = nullptr;
  for (; node != nullptr; node = node->parent()) {
    if (node->abort()) {
      aborted_parent = node;
    }
  }
  return aborted_parent;
}

abstract_node_t* abstract_node_t::get_next_node_after_abort_and_reset_until(abstract_node_t* node,
                                                                            control_node_t* until) {
  while (node != until) {
    node->internal_reset();
    node = node->parent();
  }
  return get_next_node_after_failure_and_reset(node);
}

} // namespace robocin

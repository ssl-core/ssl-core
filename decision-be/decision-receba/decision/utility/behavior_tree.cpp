#include "decision/utility/behavior_tree.h"

#if __has_include(<cxxabi.h>)
#define ROBOCIN_UTILITY_HAS_CXX_ABI_SUPPORT
#include <cxxabi.h>
#endif

namespace robocin::behavior_tree_internal {

std::string demangle_or_default(const char* mangled) {
#ifdef ROBOCIN_UTILITY_HAS_CXX_ABI_SUPPORT
  if (char* c_str_name = abi::__cxa_demangle(mangled, nullptr, nullptr, nullptr)) {
    std::string name{c_str_name};
    std::free(c_str_name); // NOLINT(*malloc*)

    return name;
  }
#endif
  return mangled;
}

} // namespace robocin::behavior_tree_internal

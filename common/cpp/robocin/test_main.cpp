#include "robocin/parameters/parameters.h"
#include "robocin/utility/nameof.h"

using robocin::ParamView;
using robocin::Singleton;

const auto pThirdPartyIp = ParamView<"third_party::network::ip">::asBool(); // NOLINT(*naming*)
const auto pIp = ParamView<"ip">::asBool();

static constexpr const char* intName = robocin::nameof<int>();

int main() {
  std::cout << intName << '\n';
  std::cout << robocin::nameof<int>() << '\n';
  std::cout << robocin::nameof<int64_t>() << '\n';
  std::cout << robocin::nameof<ParamView<"ip">>() << '\n';
  std::cout << robocin::nameof<std::vector<int>>() << '\n';
  return 0;

  // std::cout << ParamView<"ip">::index() << '\n';
  auto locs = ParamView<"ip">::locations();

  std::cout << "ROBOCIN_PROJECT_PATH"
            << ": " << ROBOCIN_PROJECT_PATH << '\n';

  for (const auto& loc : locs) {
    std::cout << "file: " << std::string{loc.file_name()}.substr(sizeof(ROBOCIN_PROJECT_PATH))
              << ':' << loc.line() << ':' << loc.column() << " `" << loc.function_name() << "`\n";
  }

  for (auto&& [key, value] : Singleton<robocin::parameters_internal::Handler>::get().parameters) {
    std::cout << key << ": " << value << '\n';
  }

  Singleton<int>::get() = 42;

  return 0;
}

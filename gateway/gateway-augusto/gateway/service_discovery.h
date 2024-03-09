#ifndef GATEWAY_SERVICE_DISCOVERY_H
#define GATEWAY_SERVICE_DISCOVERY_H

#include "gateway/nlohmann/json.hpp"
#include "gateway/utility/singleton.h"
#include "nlohmann/json.hpp"

namespace gateway {
namespace service_discovery_internal {

class ServiceDiscovery {
 public:
  struct Args {
    std::string address;
    std::vector<std::string> topics;
  };

  ServiceDiscovery() = default;

  void setup(const nlohmann::json& service_domain);
  const Args& lookup(std::string_view service_name) const;

 private:
  std::unordered_map<std::string, Args> service_names_;
};

} // namespace service_discovery_internal

// NOLINTNEXTLINE(*naming*, *const*)
static auto& SServiceDiscovery
    = Singleton<service_discovery_internal::ServiceDiscovery>::get();

} // namespace gateway

#endif // GATEWAY_SERVICE_DISCOVERY_H

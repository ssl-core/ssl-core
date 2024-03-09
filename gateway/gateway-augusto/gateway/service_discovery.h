#ifndef GATEWAY_SERVICE_DISCOVERY_H
#define GATEWAY_SERVICE_DISCOVERY_H

#include "gateway/nlohmann/json.hpp"
#include "gateway/singleton.h"
#include "nlohmann/json.hpp"

namespace gateway {

namespace service_discovery_internal {

class ServiceDiscovery {
  struct Args {
    std::string address;
    std::vector<std::string> topics;
  };

 public:
  ServiceDiscovery() = default;

  void setup(const nlohmann::json& service_domain);
  const Args& lookup(std::string_view service_name) const;

 private:
  static Args toArgs(const nlohmann::json& json);

  std::unordered_map<std::string, Args> service_names_;
};

} // namespace service_discovery_internal

// NOLINTNEXTLINE(*naming*, *const*)
static auto& SServiceDiscovery
    = Singleton<service_discovery_internal::ServiceDiscovery>::getInstance();

} // namespace gateway

#endif // GATEWAY_SERVICE_DISCOVERY_H

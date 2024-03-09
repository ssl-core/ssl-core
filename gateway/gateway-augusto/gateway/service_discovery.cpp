#include "gateway/service_discovery.h"

#include <format>
#include <stdexcept>

namespace gateway::service_discovery_internal {
namespace {

ServiceDiscovery::Args toArgs(const nlohmann::json& json) {
  std::string address = json["address"].get<std::string>();

  std::vector<std::string> topics;
  if (json.contains("topics")) {
    json["topics"].get_to(topics);
  }

  return {
      .address = address,
      .topics = topics,
  };
}

} // namespace

void ServiceDiscovery::setup(const nlohmann::json& service_domain) {
  service_names_.reserve(service_domain.size());

  for (const auto& [key, value] : service_domain.items()) {
    service_names_.emplace(key, toArgs(value));
  }
}

const ServiceDiscovery::Args& ServiceDiscovery::lookup(std::string_view service_name) const {
  auto it = service_names_.find(std::string{service_name});
  if (it == service_names_.end()) {
    throw std::runtime_error(
        std::format("ServiceDiscovery: service name '{}' not found.", service_name));
  }
  return it->second;
}

} // namespace gateway::service_discovery_internal

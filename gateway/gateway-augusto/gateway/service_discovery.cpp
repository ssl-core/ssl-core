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

void ServiceDiscovery::setup(const nlohmann::json& service_registry) {
  service_names_.reserve(service_registry.size());

  for (const auto& [key, value] : service_registry.items()) {
    service_names_.emplace(key, toArgs(value));
  }
}

const ServiceDiscovery::Args& ServiceDiscovery::lookup(std::string_view service_name) const {
  auto found_service_it = service_names_.find(std::string{service_name});
  if (found_service_it == service_names_.end()) {
    throw std::runtime_error(
        std::format("ServiceDiscovery: service name '{}' not found.", service_name));
  }
  return found_service_it->second;
}

} // namespace gateway::service_discovery_internal

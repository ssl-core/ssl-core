#include "vision/db/repository_factory_mapping.h"

#include "vision/db/irepository_factory.h"
#include "vision/db/mongodb_repository_factory.h"

namespace vision {

RepositoryFactoryMapping::RepositoryFactoryMapping() {
  factories_.emplace(RepositoryType::MongoDb,
                     []() { return std::make_unique<MongoDbRepositoryFactory>(); });
}

std::unique_ptr<IRepositoryFactory>
RepositoryFactoryMapping::operator[](RepositoryType repository_type) const {
  if (auto it = factories_.find(repository_type); it != factories_.end()) {
    const std::function<std::unique_ptr<IRepositoryFactory>()>& factory = it->second;
    return factory();
  }
  return nullptr;
}

} // namespace vision

// RepositoryFactoryMapping.cpp
#include "repository_factory_mapping.h"

#include "../repository_factory_mongo_db/repository_factory_mongo_db.h"

RepositoryFactoryMapping::RepositoryFactoryMapping() {
  factoryMapping_[RepositoryType::MONGODB] = std::make_shared<RepositoryFactoryMongoDB>();
}

std::shared_ptr<RepositoryAbstractFactory>
RepositoryFactoryMapping::getFactory(RepositoryType repository_type) const {
  auto it = factoryMapping_.find(repository_type);
  if (it != factoryMapping_.end()) {
    return it->second;
  }
  return nullptr;
}

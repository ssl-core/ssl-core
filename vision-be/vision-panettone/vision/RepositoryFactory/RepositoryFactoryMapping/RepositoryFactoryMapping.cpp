// RepositoryFactoryMapping.cpp
#include "RepositoryFactoryMapping.h"

#include "../RepositoryFactoryMongoDB/RepositoryFactoryMongoDB.h"

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

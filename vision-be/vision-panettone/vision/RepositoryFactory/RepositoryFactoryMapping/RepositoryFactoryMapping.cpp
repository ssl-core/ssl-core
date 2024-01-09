// RepositoryFactoryMapping.cpp
#include "RepositoryFactoryMapping.h"

RepositoryFactoryMapping::RepositoryFactoryMapping() {
  factoryMapping_[RepositoryType::MONGODB] = new RepositoryFactoryMongoDB();
}

RepositoryAbstractFactory*
RepositoryFactoryMapping::getFactory(RepositoryType repositoryType) const {
  auto it = factoryMapping.find(repositoryType);
  if (it != factoryMapping.end()) {
    return it->second;
  }
  return nullptr; // Or handle the case when the repository type is not found
}

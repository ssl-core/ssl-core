// RepositoryFactoryMapping.h
#ifndef REPOSITORY_FACTORY_MAPPING_H
#define REPOSITORY_FACTORY_MAPPING_H

#include "../RepositoryAbstractFactory.h"
#include "../RepositoryType.h"

#include <unordered_map>

class RepositoryFactoryMapping {
 public:
  RepositoryFactoryMapping();
  [[nodiscard]] RepositoryAbstractFactory* getFactory(RepositoryType repository_type) const;

 private:
  std::unordered_map<RepositoryType, RepositoryAbstractFactory*> factoryMapping_;
};

#endif // REPOSITORY_FACTORY_MAPPING_H

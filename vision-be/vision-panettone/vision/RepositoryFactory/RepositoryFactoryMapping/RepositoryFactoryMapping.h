// RepositoryFactoryMapping.h
#ifndef REPOSITORY_FACTORY_MAPPING_H
#define REPOSITORY_FACTORY_MAPPING_H

#include "../RepositoryAbstractFactory.h"
#include "../RepositoryType.h"

#include <memory>
#include <unordered_map>

class RepositoryFactoryMapping {
 public:
  RepositoryFactoryMapping();
  [[nodiscard]] std::shared_ptr<RepositoryAbstractFactory>
  getFactory(RepositoryType repository_type) const;

 private:
  std::unordered_map<RepositoryType, std::shared_ptr<RepositoryAbstractFactory>> factoryMapping_;
};

#endif // REPOSITORY_FACTORY_MAPPING_H

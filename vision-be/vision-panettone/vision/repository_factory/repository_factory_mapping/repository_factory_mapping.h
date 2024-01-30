// RepositoryFactoryMapping.h
#ifndef VISION_REPOSITORY_FACTORY_REPOSITORY_FACTORY_MAPPING_REPOSITORY_FACTORY_MAPPING_H
#define VISION_REPOSITORY_FACTORY_REPOSITORY_FACTORY_MAPPING_REPOSITORY_FACTORY_MAPPING_H

#include "vision/repository_factory/repository_abstract_factory.h"

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

#endif // VISION_REPOSITORY_FACTORY_REPOSITORY_FACTORY_MAPPING_REPOSITORY_FACTORY_MAPPING_H

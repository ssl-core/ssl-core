#ifndef VISION_DB_REPOSITORY_FACTORY_MAPPING_H
#define VISION_DB_REPOSITORY_FACTORY_MAPPING_H

#include "vision/db/irepository_factory.h"

#include <unordered_map>

namespace vision {

// TODO($ISSUE_N): Update to Singleton pattern.
class RepositoryFactoryMapping {
 public:
  RepositoryFactoryMapping();

  [[nodiscard]] std::unique_ptr<IRepositoryFactory>
  operator[](RepositoryType repository_type) const;

 private:
  // TODO($ISSUE_N): Use direct access instead of std::unordered_map.
  std::unordered_map<RepositoryType, std::function<std::unique_ptr<IRepositoryFactory>()>>
      factories_;
};

} // namespace vision

#endif // VISION_DB_REPOSITORY_FACTORY_MAPPING_H

#ifndef VISION_DB_IREPOSITORY_FACTORY_H
#define VISION_DB_IREPOSITORY_FACTORY_H

#include "vision/db/iframe_repository.h"

#include <memory>

namespace vision {

enum class RepositoryType {
  MongoDb,
};

class IRepositoryFactory {
 public:

  [[nodiscard]] virtual std::unique_ptr<IFrameRepository> createFrameRepository() const = 0;

  [[nodiscard]] virtual std::unique_ptr<IFrameRepository> createFrameRepository(const int id) const = 0;

  // TODO($ISSUE_N): Add other repositories for Ball, Robot...

  virtual ~IRepositoryFactory() = default;
};

} // namespace vision

#endif // VISION_DB_IREPOSITORY_FACTORY_H

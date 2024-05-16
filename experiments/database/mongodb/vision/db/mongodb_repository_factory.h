#ifndef VISION_DB_MONGODB_REPOSITORY_FACTORY_H
#define VISION_DB_MONGODB_REPOSITORY_FACTORY_H

#include "vision/db/iframe_repository.h"
#include "vision/db/irepository_factory.h"

namespace vision {

class MongoDbRepositoryFactory : public IRepositoryFactory {
 public:
  MongoDbRepositoryFactory() = default;

  [[nodiscard]] std::unique_ptr<IFrameRepository> createFrameRepository() const override;

  [[nodiscard]] std::unique_ptr<IFrameRepository>
  createFrameRepository(const int id) const override;
};

} // namespace vision

#endif // VISION_DB_MONGODB_REPOSITORY_FACTORY_H

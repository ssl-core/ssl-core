// RepositoryFactoryMongoDB.h
#ifndef VISION_REPOSITORY_FACTORY_REPOSITORY_FACTORY_MONGO_DB_REPOSITORY_FACTORY_MONGO_DB_H
#define VISION_REPOSITORY_FACTORY_REPOSITORY_FACTORY_MONGO_DB_REPOSITORY_FACTORY_MONGO_DB_H

#include "vision/data/frame/i_frame_repository.h"
#include "vision/repository_factory/repository_abstract_factory.h"

class RepositoryFactoryMongoDB : public RepositoryAbstractFactory {
 public:
  RepositoryFactoryMongoDB();

  [[nodiscard]] std::unique_ptr<IFrameRepository> createFrameRepository() const override;
};

#endif // VISION_REPOSITORY_FACTORY_REPOSITORY_FACTORY_MONGO_DB_REPOSITORY_FACTORY_MONGO_DB_H

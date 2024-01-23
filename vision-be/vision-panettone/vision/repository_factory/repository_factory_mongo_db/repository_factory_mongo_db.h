// RepositoryFactoryMongoDB.h
#ifndef REPOSITORY_FACTORY_MONGODB_H
#define REPOSITORY_FACTORY_MONGODB_H

#include "../repository_abstract_factory.h"
#include "vision/data/frame/i_frame_repository.h"

class RepositoryFactoryMongoDB : public RepositoryAbstractFactory {
 public:
  RepositoryFactoryMongoDB();

  [[nodiscard]] std::unique_ptr<IFrameRepository> createFrameRepository() const override;
};

#endif // REPOSITORY_FACTORY_MONGODB_H

// RepositoryFactoryMongoDB.h
#ifndef REPOSITORY_FACTORY_MONGODB_H
#define REPOSITORY_FACTORY_MONGODB_H

#include "../RepositoryAbstractFactory.h"
#include "vision/data/Frame/IFrameRepository.h"

class RepositoryFactoryMongoDB : public RepositoryAbstractFactory {
 public:
  RepositoryFactoryMongoDB();

  [[nodiscard]] std::unique_ptr<IFrameRepository> createFrameRepository() const override;
};

#endif // REPOSITORY_FACTORY_MONGODB_H

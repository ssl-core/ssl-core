// RepositoryFactoryMongoDB.h
#ifndef REPOSITORY_FACTORY_MONGODB_H
#define REPOSITORY_FACTORY_MONGODB_H

#include "../RepositoryAbstractFactory.h"
#include "data/FrameRepositoryMongoDB/FrameRepositoryMongoDB.h"

class RepositoryFactoryMongoDB : public RepositoryAbstractFactory {
 public:
  [[nodiscard]] IFrameRepository* createFrameRepository() const override;
};

#endif // REPOSITORY_FACTORY_MONGODB_H

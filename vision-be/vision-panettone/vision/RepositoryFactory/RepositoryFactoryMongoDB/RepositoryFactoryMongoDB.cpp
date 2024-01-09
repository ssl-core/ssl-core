// RepositoryFactoryMongoDB.cpp
#include "RepositoryFactoryMongoDB.h"

IFrameRepository* RepositoryFactoryMongoDB::createFrameRepository() const {
  return new FrameRepositoryMongoDB();
}

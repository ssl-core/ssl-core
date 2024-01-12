// RepositoryFactoryMongoDB.cpp
#include "RepositoryFactoryMongoDB.h"

#include "../../data/Frame/FrameRepositoryMongoDB/FrameRepositoryMongoDB.h"
#include "../../data/Frame/IFrameRepository.h"

std::unique_ptr<IFrameRepository> RepositoryFactoryMongoDB::createFrameRepository() const {
  return std::make_unique<FrameRepositoryMongoDB>();
}

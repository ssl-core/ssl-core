// RepositoryFactoryMongoDB.cpp
#include "vision/repository_factory/repository_factory_mongo_db/repository_factory_mongo_db.h"

#include "vision/data/frame/frame_repository_mongo_db/frame_repository_mongo_db.h"

std::unique_ptr<IFrameRepository> RepositoryFactoryMongoDB::createFrameRepository() const {
  return std::make_unique<FrameRepositoryMongoDB>();
}

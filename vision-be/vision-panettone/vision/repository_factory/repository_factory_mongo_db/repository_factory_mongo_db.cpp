// RepositoryFactoryMongoDB.cpp
#include "repository_factory_mongo_db.h"

#include "../../data/frame/frame_repository_mongo_db/frame_repository_mongo_db.h"
#include "../../data/frame/i_frame_repository.h"

std::unique_ptr<IFrameRepository> RepositoryFactoryMongoDB::createFrameRepository() const {
  return std::make_unique<FrameRepositoryMongoDB>();
}

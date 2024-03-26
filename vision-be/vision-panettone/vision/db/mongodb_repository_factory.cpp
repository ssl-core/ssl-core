#include "vision/db/mongodb_repository_factory.h"

#include "vision/db/frame_repository_mongodb.h"

namespace vision {

std::unique_ptr<IFrameRepository> MongoDbRepositoryFactory::createFrameRepository() const {
  return std::make_unique<FrameRepositoryMongoDb>(MongoDbRepositoryBuildArgs{
      .uri = "mongodb://mongodb:27017/",
      .db_name = "frames_database",
      .collection_name = "frames_collection",
  });
}

} // namespace vision

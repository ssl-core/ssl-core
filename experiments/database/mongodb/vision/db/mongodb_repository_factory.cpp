#include "vision/db/mongodb_repository_factory.h"

#include "vision/db/frame_repository_mongodb.h"

namespace vision {

std::unique_ptr<IFrameRepository> MongoDbRepositoryFactory::createFrameRepository() const {
  return std::make_unique<FrameRepositoryMongoDb>(MongoDbRepositoryBuildArgs{
      .uri = "mongodb://mongodb1:27017,mongodb2:27018,mongodb3:27019/?replicaSet=rs0",
      .db_name = "frames_database",
      .collection_name = "frames_collection",
  });
}

std::unique_ptr<IFrameRepository>
MongoDbRepositoryFactory::createFrameRepository(const int id) const {
  return std::make_unique<FrameRepositoryMongoDb>(MongoDbRepositoryBuildArgs{
      .uri = "mongodb://mongodb1:27017,mongodb2:27018,mongodb3:27019/?replicaSet=rs0",
      .db_name = std::format("{}_frames_database", id),
      .collection_name = "frames_collection",
  });
}

} // namespace vision

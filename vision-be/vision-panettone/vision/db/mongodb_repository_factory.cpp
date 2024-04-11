#include "vision/db/mongodb_repository_factory.h"

#include "vision/db/frame_repository_mongodb.h"

namespace vision {

std::unique_ptr<IFrameRepository> MongoDbRepositoryFactory::createFrameRepository() const {
  return std::make_unique<FrameRepositoryMongoDb>(MongoDbRepositoryBuildArgs{
      .uri = "mongodb://ssl-core-mongodb1-1:27017,ssl-core-mongodb2-1:27018,ssl-core-mongodb3-1:27019/?replicaSet=rs0",
      .db_name = "frames_database",
      .collection_name = "frames_collection",
  });
}

} // namespace vision

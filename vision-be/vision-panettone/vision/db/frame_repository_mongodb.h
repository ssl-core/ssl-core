#ifndef VISION_DB_FRAME_REPOSITORY_MONGODB_H
#define VISION_DB_FRAME_REPOSITORY_MONGODB_H

#include "protocols/vision/frame.pb.h"
#include "vision/db/imongodb_repository.h"

namespace vision {

class FrameRepositoryMongoDb : public IMongoDbRepository<int64_t, protocols::vision::Frame> {
 public:
  explicit FrameRepositoryMongoDb(const MongoDbRepositoryBuildArgs& args);

  ~FrameRepositoryMongoDb() override = default;

  void save(const protocols::vision::Frame& frame) final;
  void remove(const int64_t& key) final;

  [[nodiscard]] std::optional<protocols::vision::Frame> find(const int64_t& key) final;
  [[nodiscard]] std::optional<std::vector<protocols::vision::Frame>> findRange(const int64_t& key_lower_bound,
                                                             const int64_t& key_upper_bound) final;
};

} // namespace vision

#endif // VISION_DB_FRAME_REPOSITORY_MONGODB_H

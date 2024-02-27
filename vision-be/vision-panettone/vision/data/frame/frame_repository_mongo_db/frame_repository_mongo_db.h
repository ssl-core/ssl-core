// FrameRepositoryMongoDB.h
#ifndef VISION_DATA_FRAME_FRAME_REPOSITORY_MONGO_DB_FRAME_REPOSITORY_MONGO_DB_H
#define VISION_DATA_FRAME_FRAME_REPOSITORY_MONGO_DB_FRAME_REPOSITORY_MONGO_DB_H

#include "vision/data/frame/i_frame_repository.h"

#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>

class FrameRepositoryMongoDB : public IFrameRepository {
 public:
  FrameRepositoryMongoDB();

  void save(const Frame& frame) override;
  [[nodiscard]] std::optional<Frame> find(const std::string& frame_id) override;
  std::optional<Frame> update(const std::string& frame_id,
                              bsoncxx::document::value& update) override;
  void remove(const std::string& frame_id) override;

 private:
  mongocxx::instance instance_;
  mongocxx::client client_;
  mongocxx::database db_;
  mongocxx::collection collection_;
};

#endif // VISION_DATA_FRAME_FRAME_REPOSITORY_MONGO_DB_FRAME_REPOSITORY_MONGO_DB_H

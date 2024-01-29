// FrameRepositoryMongoDB.h
#ifndef FRAME_REPOSITORY_MONGODB_H
#define FRAME_REPOSITORY_MONGODB_H

#include "../i_frame_repository.h"

#include <mongocxx/client.hpp>

class FrameRepositoryMongoDB : public IFrameRepository {
 public:
  FrameRepositoryMongoDB();

  void save(const Frame& frame) override;
  [[nodiscard]] std::optional<Frame> find(const std::string& frame_id) override;
  std::optional<Frame> update(const std::string& frame_id,
                              bsoncxx::document::value& update) override;
  void remove(const std::string& frame_id) override;

 private:
  mongocxx::client client_;
  mongocxx::database db_;
  mongocxx::collection collection_;
};

#endif // FRAME_REPOSITORY_MONGODB_H

// FrameRepositoryMongoDB.h
#ifndef FRAME_REPOSITORY_MONGODB_H
#define FRAME_REPOSITORY_MONGODB_H

#include "../../../src/Frame/Frame.h"
#include "../IFrameRepository.h"

#include <mongocxx/client.hpp>

class FrameRepositoryMongoDB : public IFrameRepository {
 public:
  FrameRepositoryMongoDB();

  void save(const Frame& frame) override;
  [[nodiscard]] std::optional<Frame> find(const std::string& frame_id) override;
  Frame update(const Frame& frame) override;
  void remove(const Frame& frame) override;

 private:
  mongocxx::client client_;
  mongocxx::database db_;
  mongocxx::collection collection_;
};

#endif // FRAME_REPOSITORY_MONGODB_H

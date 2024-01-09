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
  Frame find(const std::string& frame_id);

 private:
  mongocxx::client client_;
  mongocxx::database db_;
  mongocxx::collection collection_;
};

#endif // FRAME_REPOSITORY_MONGODB_H

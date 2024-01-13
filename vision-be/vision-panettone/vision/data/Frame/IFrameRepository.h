// IFrameRepository.h
#ifndef IFRAME_REPOSITORY_H
#define IFRAME_REPOSITORY_H

#include "../../src/Frame/Frame.h"

#include <mongocxx/collection.hpp>
#include <optional>

class IFrameRepository {
 public:
  IFrameRepository() = default;
  virtual ~IFrameRepository() = default;

  IFrameRepository(const IFrameRepository&) = delete;
  IFrameRepository& operator=(const IFrameRepository&) = delete;
  IFrameRepository(IFrameRepository&&) = delete;
  IFrameRepository& operator=(IFrameRepository&&) = delete;

  virtual void save(const Frame& frame) = 0;
  [[nodiscard]] virtual std::optional<Frame> find(const std::string& frame_id) = 0;
  virtual std::optional<Frame> update(const std::string& frame_id, bsoncxx::document::value& update)
      = 0;
  virtual void remove(const std::string& frame_id) = 0;
};

#endif // IFRAME_REPOSITORY_H

// IFrameRepository.h
#ifndef IFRAME_REPOSITORY_H
#define IFRAME_REPOSITORY_H

#include "../../src/Frame/Frame.h"

class IFrameRepository {
 public:
  virtual ~IFrameRepository() = default;

  IFrameRepository(const IFrameRepository&) = delete;
  IFrameRepository& operator=(const IFrameRepository&) = delete;
  IFrameRepository(IFrameRepository&&) = delete;
  IFrameRepository& operator=(IFrameRepository&&) = delete;

  virtual void save(const Frame& frame) = 0;
  [[nodiscard]] virtual Frame find(const std::string& frame_id) const = 0;
  virtual Frame update(const Frame& frame) = 0;
  virtual void remove(const Frame& frame) = 0;
};

#endif // IFRAME_REPOSITORY_H

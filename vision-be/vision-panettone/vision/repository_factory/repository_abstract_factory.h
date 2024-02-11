// RepositoryAbstractFactory.h
#ifndef VISION_REPOSITORY_FACTORY_REPOSITORY_ABSTRACT_FACTORY_H
#define VISION_REPOSITORY_FACTORY_REPOSITORY_ABSTRACT_FACTORY_H

#include "vision/data/frame/i_frame_repository.h"

#include <memory>

enum class RepositoryType {
  MONGODB
};

class RepositoryAbstractFactory {
 public:
  [[nodiscard]] virtual std::unique_ptr<IFrameRepository> createFrameRepository() const = 0;
  // TODO(repositories): Create other repositories for Ball, Robot, etc.

  RepositoryAbstractFactory() = default;
  virtual ~RepositoryAbstractFactory() = default;

  RepositoryAbstractFactory(const RepositoryAbstractFactory&) = default;
  RepositoryAbstractFactory& operator=(const RepositoryAbstractFactory&) = default;
  RepositoryAbstractFactory(RepositoryAbstractFactory&&) = default;
  RepositoryAbstractFactory& operator=(RepositoryAbstractFactory&&) = default;
};

#endif // VISION_REPOSITORY_FACTORY_REPOSITORY_ABSTRACT_FACTORY_H

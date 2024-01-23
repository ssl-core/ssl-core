// RepositoryAbstractFactory.h
#ifndef REPOSITORY_ABSTRACT_FACTORY_H
#define REPOSITORY_ABSTRACT_FACTORY_H

#include "../data/frame/i_frame_repository.h"

#include <memory>

class RepositoryAbstractFactory {
 public:
  [[nodiscard]] virtual std::unique_ptr<IFrameRepository> createFrameRepository() const = 0;
  // TODO(repositories): Create other repositories for Ball, Robot, etc.

  virtual ~RepositoryAbstractFactory() = default;

  RepositoryAbstractFactory(const RepositoryAbstractFactory&) = default;
  RepositoryAbstractFactory& operator=(const RepositoryAbstractFactory&) = default;
  RepositoryAbstractFactory(RepositoryAbstractFactory&&) = default;
  RepositoryAbstractFactory& operator=(RepositoryAbstractFactory&&) = default;
};

#endif // REPOSITORY_ABSTRACT_FACTORY_H

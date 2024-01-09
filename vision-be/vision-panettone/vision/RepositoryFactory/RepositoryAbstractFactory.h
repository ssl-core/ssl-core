// RepositoryAbstractFactory.h
#ifndef REPOSITORY_ABSTRACT_FACTORY_H
#define REPOSITORY_ABSTRACT_FACTORY_H

#include "../data/Frame/IFrameRepository.h"

class RepositoryAbstractFactory {
 public:
  [[nodiscard]] virtual IFrameRepository* createFrameRepository() const = 0;
  virtual ~RepositoryAbstractFactory() = default;

  RepositoryAbstractFactory(const RepositoryAbstractFactory&) = default;
  RepositoryAbstractFactory& operator=(const RepositoryAbstractFactory&) = default;
  RepositoryAbstractFactory(RepositoryAbstractFactory&&) = default;
  RepositoryAbstractFactory& operator=(RepositoryAbstractFactory&&) = default;
};

#endif // REPOSITORY_ABSTRACT_FACTORY_H

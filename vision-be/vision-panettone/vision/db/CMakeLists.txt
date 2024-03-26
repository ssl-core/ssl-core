robocin_cpp_library(
  NAME irepository
  HDRS irepository.h
  SRCS irepository.cpp
)

robocin_cpp_library(
  NAME imongodb_repository
  HDRS imongodb_repository.h
  SRCS imongodb_repository.cpp
  DEPS irepository ${mongocxx_deps}
)

robocin_cpp_library(
  NAME iframe_repository
  HDRS iframe_repository.h
  SRCS iframe_repository.cpp
  DEPS protocols::vision irepository
)

robocin_cpp_library(
  NAME frame_repository_mongodb
  HDRS frame_repository_mongodb.h
  SRCS frame_repository_mongodb.cpp
  DEPS protocols::vision imongodb_repository
)

robocin_cpp_library(
  NAME irepository_factory
  HDRS irepository_factory.h
  SRCS irepository_factory.cpp
  DEPS iframe_repository
)

robocin_cpp_library(
  NAME mongodb_repository_factory
  HDRS mongodb_repository_factory.h
  SRCS mongodb_repository_factory.cpp
  DEPS irepository_factory frame_repository_mongodb
)

robocin_cpp_library(
  NAME repository_factory_mapping
  HDRS repository_factory_mapping.h
  SRCS repository_factory_mapping.cpp
  DEPS irepository_factory mongodb_repository_factory
)

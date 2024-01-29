add_subdirectory(repository_factory_mapping)
add_subdirectory(repository_factory_mongo_db)

robocin_cpp_library(
  NAME repository_factory
  HDRS repository_abstract_factory.h repository_type.h
  DEPS frame_repository
)

add_subdirectory(repository_factory_mapping)
add_subdirectory(repository_factory_mongo_db)

robocin_cpp_library(
  NAME repository_factory
  DEPS repository_abstract_factory.h repository_type.h
)

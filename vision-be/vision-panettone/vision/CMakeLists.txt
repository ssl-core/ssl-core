robocin_cpp_executable(
  NAME vision_main
  SRCS vision_main.cpp
  # TODO: Use static libraries instead.
  DEPS repository_factory_mapping mongo::mongoc_shared mongo::bson_shared mongo::mongocxx_shared mongo::bsoncxx_shared thread_pool
)

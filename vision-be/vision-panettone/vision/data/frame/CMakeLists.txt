robocin_cpp_library(
  NAME frame_repository
  HDRS i_frame_repository.h
  SRCS frame_repository.cpp
  DEPS frame mongo::mongoc_shared mongo::bson_shared mongo::mongocxx_shared mongo::bsoncxx_shared
)

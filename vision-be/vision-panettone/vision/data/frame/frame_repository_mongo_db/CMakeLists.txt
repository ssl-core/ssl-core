robocin_cpp_library(
  NAME frame_repository_mongo_db
  HDRS frame_repository_mongo_db.h
  SRCS frame_repository_mongo_db.cpp
  DEPS frame_repository mongo::mongoc_shared mongo::bson_shared mongo::mongocxx_shared mongo::bsoncxx_shared
)

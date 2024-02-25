robocin_cpp_executable(
  NAME service_main
  SRCS service_main.cpp
  DEPS mongo::mongoc_static mongo::bson_static mongo::mongocxx_static mongo::bsoncxx_static
)

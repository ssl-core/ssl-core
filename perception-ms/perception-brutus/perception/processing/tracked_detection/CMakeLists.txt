robocin_cpp_library(
  NAME tracked_detection
  HDRS filters/tracked_detection_filter.h
       mappers/tracked_detection_mapper.h
  SRCS filters/tracked_detection_filter.cpp
       mappers/tracked_detection_mapper.cpp
  DEPS parameters protocols::protobufs common::third_party_adaptors
)

robocin_cpp_test(
  NAME tracked_detection_mapper_test
  SRCS mappers/tracked_detection_mapper_test.cpp
  DEPS tracked_detection common::third_party_protobuf
)

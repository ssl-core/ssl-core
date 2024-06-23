robocin_cpp_library(
  NAME tracked_detection
  HDRS filters/tracked_detection_filter.h
       mappers/tracked_detection_mapper.h
  SRCS filters/tracked_detection_filter.cpp
       mappers/tracked_detection_mapper.cpp
  DEPS protocols::protobufs
)

# robocin_cpp_test(
#   NAME tracked_detection_mapper_test
#   SRCS mappers/tracked_detection_mapper_test.cpp
#   DEPS mappers/tracked_detection_mapper
# )


robocin_cpp_library(
  NAME raw_detection
  HDRS entities/raw_ball.h
       entities/raw_robot.h
       entities/robot_id.h
       entities/raw_detection.h
       filters/ball_filter.h
       filters/ball_filter_factory.h
       filters/robot_filter.h
       filters/robot_filter_factory.h
       filters/camera_filter.h
       filters/camera_filter_factory.h
       filters/raw_detection_filter.h
       mappers/raw_detection_mapper.h
  SRCS entities/raw_ball.cpp
       entities/raw_robot.cpp
       entities/robot_id.cpp
       entities/raw_detection.cpp
       filters/ball_filter.cpp
       filters/ball_filter_factory.cpp
       filters/robot_filter.cpp
       filters/robot_filter_factory.cpp
       filters/camera_filter.cpp
       filters/camera_filter_factory.cpp
       filters/raw_detection_filter.cpp
       mappers/raw_detection_mapper.cpp
  DEPS common::geometry
       common::output
       protocols::protobufs
)

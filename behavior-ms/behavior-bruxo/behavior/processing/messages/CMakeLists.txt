robocin_cpp_library(
    NAME messages
    HDRS behavior/behavior_message.h
         common/robot_id/robot_id.h
         motion/motion_message.h
         planning/planning_message.h
    SRCS behavior/behavior_message.cpp
         common/robot_id/robot_id.cpp
         motion/motion_message.cpp
         planning/planning_message.cpp
    DEPS common::utility
         common::output
         common::geometry
)
robocin_cpp_executable(
  NAME vision_main
  SRCS vision_main.cpp
  DEPS protocols::vision
       irepository_factory
       repository_factory_mapping
)

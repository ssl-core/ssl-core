add_subdirectory(frame_repository_mongo_db)

robocin_cpp_library(
  NAME frame_repository
  DEPS i_frame_repository.h ../../src/frame/frame.h
)

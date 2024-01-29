add_subdirectory(frame_repository_mongo_db)

robocin_cpp_library(
  NAME frame_repository
  HDRS i_frame_repository.h
  DEPS frame
)

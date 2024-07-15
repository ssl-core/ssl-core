if (Protobuf_FOUND AND absl_FOUND)
  robocin_cpp_library(
    NAME third_party_adaptors
    HDRS adaptors/pb_time_util.h
    SRCS adaptors/pb_time_util.cpp
    DEPS protobuf::libprotobuf absl::time
  )

  if (GTest_FOUND)
    robocin_cpp_library(
      NAME third_party_protobuf
      HDRS google/protobuf/test_textproto.h
      SRCS google/protobuf/test_textproto.cpp
      DEPS GTest::gmock absl::log absl::memory protobuf::libprotobuf
    )
  endif ()
endif ()

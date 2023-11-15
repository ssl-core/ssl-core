########################################################################################################################

# define the project name to be used in the code
set(ROBOCIN_PROJECT_NAME ${CMAKE_PROJECT_NAME})

if (NOT ROBOCIN_PROJECT_NAME)
  message(FATAL_ERROR "essentials: ROBOCIN_PROJECT_NAME is not defined.")
endif ()

########################################################################################################################

# set CMAKE_EXPORT_COMPILE_COMMANDS to ON to generate a compile_commands.json file in the build directory by default
# this file is used by clang tools such as clangd, clang-tidy and clang-format

if (NOT CMAKE_EXPORT_COMPILE_COMMANDS)
  set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
endif ()

########################################################################################################################

# find Threads package
find_package(Threads REQUIRED)

########################################################################################################################

# find Google Test package
# it enable the following variables:
#   GTest::gtest                          the gtest library
#   GTest::gtest_main                     the gtest_main library, which is used to link against the main function
#   GTest::gmock                          the gmock library
#   GTest::gmock_main                     the gmock_main library, which is used to link against the main function

# find GTest installation
# looks for GTest cmake config files installed by GTest's cmake installation.
find_package(GTest CONFIG REQUIRED HINTS "/opt/googletest")
message(STATUS "Using GTest: ${GTest_VERSION}")

include(GoogleTest) # provided by CMake

enable_testing() # enable testing for the entire project

########################################################################################################################

# find Google Benchmark package
# it enable the following variables:
#   benchmark::benchmark                  the benchmark library
#   benchmark::benchmark_main             the benchmark_main library, which is used to link against the main function

# find Google Benchmark installation
# looks for Google Benchmark cmake config files installed by Google Benchmark's cmake installation.
find_package(benchmark CONFIG HINTS "/opt/benchmark")
message(STATUS "Using Google Benchmark: ${benchmark_VERSION}")

########################################################################################################################

# add cpp library
# named parameters:
#  NAME: name of the library
#  HDRS: header files
#  SRCS: source files
#  MODS: module files
#  DEPS: dependencies
#  MACROS: macros
#  CONFIGS: CMake configurable files
function(robocin_cpp_library)
  cmake_parse_arguments(
          ARG                                    # prefix of output variables
          ""                                     # list of names of the boolean arguments
          "NAME"                                 # list of names of mono-valued arguments
          "HDRS;SRCS;MODS;DEPS;MACROS;CONFIGS"   # list of names of multi-valued arguments
          ${ARGN}                                # arguments of the function to parse (ARGN contains all the arguments after the function name)
  )

  # if there isn't at least one module file, then should be at least one header file and one source file
  if (NOT ARG_MODS)
    # if there isn't at least one header file, then the library is not created
    if (NOT ARG_HDRS)
      message(FATAL_ERROR "robocin_cpp_library: no header files given.")
    endif ()

    # if there isn't at least one source file, then the library is not created
    if (NOT ARG_SRCS)
      message(FATAL_ERROR "robocin_cpp_library: no source files given.")
    endif ()
  endif ()

  # if there are CMake configurable files, then they are configured and added to the library
  if (ARG_CONFIGS)
    foreach (CONFIG_FILE ${ARG_CONFIGS})
      get_filename_component(config_last_extension ${CONFIG_FILE} LAST_EXT)

      if (NOT ${config_last_extension} STREQUAL ".in")
        message(FATAL_ERROR "robocin_cpp_library: invalid extension '${config_last_extension}' for configurable file '${CONFIG_FILE}'.")
      endif ()

      get_filename_component(config_filename ${CONFIG_FILE} NAME_WLE)
      configure_file(${CONFIG_FILE} ${config_filename})
      list(APPEND CONFIG_HDRS ${CMAKE_CURRENT_BINARY_DIR}/${config_filename})
    endforeach ()
  endif ()

  add_library(${ARG_NAME} ${ARG_HDRS} ${ARG_SRCS} ${ARG_MODS} ${CONFIG_HDRS}) # add library with given name, headers, sources and modules
  target_link_libraries(${ARG_NAME} PUBLIC ${ARG_DEPS}) # link library with given dependencies

  target_include_directories(${ARG_NAME} PRIVATE ${ROBOCIN_PROJECT_PATH})
  target_include_directories(${ARG_NAME} PRIVATE ${CMAKE_BINARY_DIR})

  target_compile_definitions(${ARG_NAME} PRIVATE ROBOCIN_PROJECT_NAME="${ROBOCIN_PROJECT_NAME}")
  target_compile_definitions(${ARG_NAME} PRIVATE ROBOCIN_PROJECT_PATH="${ROBOCIN_PROJECT_PATH}")

  # target_sources(${ARG_NAME} PUBLIC FILE_SET cxx_modules TYPE CXX_MODULES FILES ${ARG_MODS})

  if (ARG_MACROS)
    target_compile_definitions(${ARG_NAME} ${ARG_MACROS})
  endif ()

endfunction(robocin_cpp_library)

########################################################################################################################

# Add cpp unit test
# Named parameters:
#  NAME: name of the test
#  HDRS: header files
#  SRCS: source files
#  MODS: module files
#  DEPS: dependencies
function(robocin_cpp_test)
  cmake_parse_arguments(
          ARG                            # prefix of output variables
          ""                             # list of names of the boolean arguments
          "NAME"                         # list of names of mono-valued arguments
          "HDRS;SRCS;MODS;DEPS;MACROS"   # list of names of multi-valued arguments
          ${ARGN}                        # arguments of the function to parse (ARGN contains all the arguments after the function name)
  )

  # check if at least one source file is given with suffix '_test.cpp'
  if (NOT ARG_SRCS)
    message(FATAL_ERROR "robocin_cpp_test: no source files given.")
  else ()
    set(FILTERED_SRCS ${ARG_SRCS})
    list(FILTER FILTERED_SRCS INCLUDE REGEX "_test\\.cpp$")

    if (NOT FILTERED_SRCS)
      message(FATAL_ERROR "robocin_cpp_test: no source files given with suffix '_test.cpp'.")
    endif ()
  endif ()

  add_executable(${ARG_NAME} ${ARG_HDRS} ${ARG_SRCS} ${ARG_MODS}) # add test with given name, headers, sources and modules
  target_link_libraries(${ARG_NAME} PRIVATE GTest::gtest GTest::gtest_main ${ARG_DEPS}) # link library with given dependencies

  target_include_directories(${ARG_NAME} PRIVATE ${ROBOCIN_PROJECT_PATH})
  target_include_directories(${ARG_NAME} PRIVATE ${CMAKE_BINARY_DIR})

  target_compile_definitions(${ARG_NAME} PRIVATE ROBOCIN_PROJECT_NAME="${ROBOCIN_PROJECT_NAME}")
  target_compile_definitions(${ARG_NAME} PRIVATE ROBOCIN_PROJECT_PATH="${ROBOCIN_PROJECT_PATH}")

  # target_sources(${ARG_NAME} PUBLIC FILE_SET cxx_modules TYPE CXX_MODULES FILES ${ARG_MODS})

  if (ARG_MACROS)
    target_compile_definitions(${ARG_NAME} ${ARG_MACROS})
  endif ()

  gtest_discover_tests(${ARG_NAME})

endfunction(robocin_cpp_test)

########################################################################################################################

# Add cpp benchmark test
# Named parameters:
#  NAME: name of the test
#  HDRS: header files
#  SRCS: source files
#  MODS: module files
#  DEPS: dependencies
function(robocin_cpp_benchmark_test)
  cmake_parse_arguments(
          ARG                            # prefix of output variables
          ""                             # list of names of the boolean arguments
          "NAME"                         # list of names of mono-valued arguments
          "HDRS;SRCS;MODS;DEPS;MACROS"   # list of names of multi-valued arguments
          ${ARGN}                        # arguments of the function to parse (ARGN contains all the arguments after the function name)
  )

  # check if at least one source file is given with suffix '_benchmark.cpp'
  if (NOT ARG_SRCS)
    message(FATAL_ERROR "robocin_cpp_benchmark_test: no source files given.")
  else ()
    set(FILTERED_SRCS ${ARG_SRCS})
    list(FILTER FILTERED_SRCS INCLUDE REGEX "_benchmark\\.cpp$")

    if (NOT FILTERED_SRCS)
      message(FATAL_ERROR "robocin_cpp_benchmark_test: no source files given with suffix '_benchmark.cpp'.")
    endif ()
  endif ()

  add_executable(${ARG_NAME} ${ARG_HDRS} ${ARG_SRCS} ${ARG_MODS}) # add test with given name, headers, sources and modules
  target_link_libraries(${ARG_NAME} PRIVATE benchmark::benchmark benchmark::benchmark_main ${ARG_DEPS}) # link library with given dependencies

  target_include_directories(${ARG_NAME} PRIVATE ${ROBOCIN_PROJECT_PATH})
  target_include_directories(${ARG_NAME} PRIVATE ${CMAKE_BINARY_DIR})

  target_compile_definitions(${ARG_NAME} PRIVATE ROBOCIN_PROJECT_NAME="${ROBOCIN_PROJECT_NAME}")
  target_compile_definitions(${ARG_NAME} PRIVATE ROBOCIN_PROJECT_PATH="${ROBOCIN_PROJECT_PATH}")

  # target_sources(${ARG_NAME} PUBLIC FILE_SET cxx_modules TYPE CXX_MODULES FILES ${ARG_MODS})

  if (ARG_MACROS)
    target_compile_definitions(${ARG_NAME} ${ARG_MACROS})
  endif ()

endfunction(robocin_cpp_benchmark_test)

########################################################################################################################

# Add cpp executable
# Named parameters:
#  NAME: name of the executable
#  HDRS: header files
#  SRCS: source files
#  MODS: module files
#  DEPS: dependencies
function(robocin_cpp_executable)
  cmake_parse_arguments(
          ARG                            # prefix of output variables
          ""                             # list of names of the boolean arguments
          "NAME"                         # list of names of mono-valued arguments
          "HDRS;SRCS;MODS;DEPS;MACROS"   # list of names of multi-valued arguments
          ${ARGN}                        # arguments of the function to parse (ARGN contains all the arguments after the function name)
  )

  # check if at least one source file is given with suffix '_main.cpp'
  if (NOT ARG_SRCS)
    message(FATAL_ERROR "robocin_cpp_executable: no source files given.")
  else ()
    set(FILTERED_SRCS ${ARG_SRCS})
    list(FILTER FILTERED_SRCS INCLUDE REGEX "_main\\.cpp$")

    if (NOT FILTERED_SRCS)
      message(FATAL_ERROR "robocin_cpp_executable: no source files given with suffix '_main.cpp'.")
    endif ()
  endif ()

  add_executable(${ARG_NAME} ${ARG_HDRS} ${ARG_SRCS} ${ARG_MODS}) # add executable with given name, headers, sources and modules
  target_link_libraries(${ARG_NAME} PRIVATE ${ARG_DEPS}) # link library with given dependencies

  target_include_directories(${ARG_NAME} PRIVATE ${ROBOCIN_PROJECT_PATH})
  target_include_directories(${ARG_NAME} PRIVATE ${CMAKE_BINARY_DIR})

  target_compile_definitions(${ARG_NAME} PRIVATE ROBOCIN_PROJECT_NAME="${ROBOCIN_PROJECT_NAME}")
  target_compile_definitions(${ARG_NAME} PRIVATE ROBOCIN_PROJECT_PATH="${ROBOCIN_PROJECT_PATH}")

  # target_sources(${ARG_NAME} PUBLIC FILE_SET cxx_modules TYPE CXX_MODULES FILES ${ARG_MODS})

  if (ARG_MACROS)
    target_compile_definitions(${ARG_NAME} ${ARG_MACROS})
  endif ()

endfunction(robocin_cpp_executable)

########################################################################################################################

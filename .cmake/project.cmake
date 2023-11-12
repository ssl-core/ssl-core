

# configure cpp project
# named parameters:
#  STANDARD: c++ standard
#  OUTPUT_DIRECTORY: output directory
macro(robocin_cpp_project_setup)
  cmake_parse_arguments(
    ARG                          # prefix of output variables
    ""                           # list of names of the boolean arguments
    "STANDARD;OUTPUT_DIRECTORY"  # list of names of mono-valued arguments
    ""                           # list of names of multi-valued arguments
    ${ARGN}                      # arguments of the function to parse (ARGN contains all the arguments after the function name)
  )

  if (NOT CMAKE_PROJECT_NAME OR CMAKE_PROJECT_NAME STREQUAL "Project")
    message(FATAL_ERROR "robocin_cpp_project_setup: project() must be called before this macro.")
  endif ()

  include(essentials)

  if (NOT CMAKE_CXX_STANDARD)
    if (ARG_STANDARD)
      set(CMAKE_CXX_STANDARD ${ARG_STANDARD})
    else ()
      message(FATAL_ERROR "robocin_cpp_project_setup: CMAKE_CXX_STANDARD not set.")
    endif ()
  endif ()

  if (NOT CMAKE_CXX_STANDARD_REQUIRED)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
  endif ()

  message(STATUS "Using C++: ${CMAKE_CXX_STANDARD}")

  if (NOT ROBOCIN_PROJECT_NAME)
    message(FATAL_ERROR "robocin_cpp_project_setup: Project name not set.")
  endif ()

  if (ARG_OUTPUT_DIRECTORY)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${ARG_OUTPUT_DIRECTORY})
  else ()
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/bin)
  endif ()

  message(STATUS "Output directory: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
  
  if (EXISTS ${CMAKE_CURRENT_LIST_DIR}/${ARG_NAME}/CMakeLists.txt)
    add_subdirectory(${ROBOCIN_PROJECT_NAME})
  else ()
    message(FATAL_ERROR "robocin_cpp_project_setup: It is expected that all source code should be present in folder ${ROBOCIN_PROJECT_NAME}.")
  endif ()

endmacro()
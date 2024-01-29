# configure cpp project
# named parameters:
#  NO_SOURCE_DIRECTORY: if set, the source directory will not be added to the project
#  STANDARD: c++ standard
#  SOURCE_DIRECTORY: source directory
#  OUTPUT_DIRECTORY: output directory
macro(robocin_cpp_project_setup)
  cmake_parse_arguments(
    ARG                                            # prefix of output variables
    "NO_SOURCE_DIRECTORY"                          # list of names of the boolean arguments
    "STANDARD;SOURCE_DIRECTORY;OUTPUT_DIRECTORY"   # list of names of mono-valued arguments
    ""                                             # list of names of multi-valued arguments
    ${ARGN}                                        # arguments of the function to parse (ARGN contains all the arguments after the function name)
  )

  if (NOT CMAKE_PROJECT_NAME OR CMAKE_PROJECT_NAME STREQUAL "Project")
    message(FATAL_ERROR "robocin_cpp_project_setup: project() must be called before this macro.")
  endif ()

  # define the project path to be used in the code
  get_filename_component(ROBOCIN_PROJECT_PATH "${CMAKE_CURRENT_LIST_DIR}" ABSOLUTE)

  include(essentials)

  if (ARG_STANDARD)
    set(CMAKE_CXX_STANDARD ${ARG_STANDARD})
  elseif (NOT CMAKE_CXX_STANDARD)
    message(FATAL_ERROR "robocin_cpp_project_setup: CMAKE_CXX_STANDARD not set.")
  endif ()

  if (NOT CMAKE_CXX_STANDARD_REQUIRED)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
  endif ()

  message(STATUS "Using C++: ${CMAKE_CXX_STANDARD}")

  if (ARG_OUTPUT_DIRECTORY)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${ARG_OUTPUT_DIRECTORY})
  else ()
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/bin)
  endif ()

  message(STATUS "Output directory: '${CMAKE_RUNTIME_OUTPUT_DIRECTORY}'")

  if (ARG_SOURCE_DIRECTORY AND ARG_NO_SOURCE_DIRECTORY)
    message(FATAL_ERROR "robocin_cpp_project_setup: cannot set both SOURCE_DIRECTORY and NO_SOURCE_DIRECTORY.")
  endif ()

  if (NOT ARG_NO_SOURCE_DIRECTORY)
    if (ARG_SOURCE_DIRECTORY)
      set(SOURCE_DIRECTORY ${ARG_SOURCE_DIRECTORY})
    else ()
      set(SOURCE_DIRECTORY ${ROBOCIN_PROJECT_NAME})
    endif ()

    if (EXISTS ${CMAKE_CURRENT_LIST_DIR}/${SOURCE_DIRECTORY}/CMakeLists.txt)
      message(STATUS "Source directory: '${CMAKE_CURRENT_LIST_DIR}/${SOURCE_DIRECTORY}'")

      # add all subdirectories of the source directory recursively
      file(GLOB_RECURSE CMAKELISTS_FILES ${CMAKE_CURRENT_LIST_DIR}/${SOURCE_DIRECTORY}/CMakeLists.txt ${CMAKE_CURRENT_LIST_DIR}/${SOURCE_DIRECTORY}/**/CMakeLists.txt)
      foreach (CMAKELISTS_FILE ${CMAKELISTS_FILES})
        get_filename_component(CMAKELISTS_FILE_PATH ${CMAKELISTS_FILE} DIRECTORY)
        add_subdirectory(${CMAKELISTS_FILE_PATH})
      endforeach ()

    else ()
      message(FATAL_ERROR "robocin_cpp_project_setup: It is expected that all source code should be present in folder '${SOURCE_DIRECTORY}'.")
    endif ()
  endif ()

endmacro()

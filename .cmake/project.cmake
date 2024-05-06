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

    # add all subdirectories of the source directory recursively
    file(GLOB_RECURSE CMAKELISTS_FILES ${CMAKE_CURRENT_LIST_DIR}/${SOURCE_DIRECTORY}/CMakeLists.txt ${CMAKE_CURRENT_LIST_DIR}/${SOURCE_DIRECTORY}/**/CMakeLists.txt)

    if (CMAKELISTS_FILES)
      message(STATUS "Source directory: '${CMAKE_CURRENT_LIST_DIR}/${SOURCE_DIRECTORY}'")

      foreach (CMAKELISTS_FILE ${CMAKELISTS_FILES})
        get_filename_component(CMAKELISTS_FILE_PATH ${CMAKELISTS_FILE} DIRECTORY)
        add_subdirectory(${CMAKELISTS_FILE_PATH})
      endforeach ()

    else ()
      message(FATAL_ERROR "robocin_cpp_project_setup: It is expected that all source code should be present in folder '${SOURCE_DIRECTORY}'.")
    endif ()
  endif ()

endmacro()

# additional mbed-os project setup
# named parameters:
#  TARGET: the microcontroller
#  BUILD_TYPE: the microcontroller dependencies build type (default: CMAKE_BUILD_TYPE)
#  TOOLCHAIN: the mbed-os toolchain
macro(robocin_mbed_setup)
  cmake_parse_arguments(
    ARG                                  # prefix of output variables
    ""                                   # list of names of the boolean arguments
    "TARGET;BUILD_TYPE;TOOLCHAIN"      # list of names of mono-valued arguments
    ""                                   # list of names of multi-valued arguments
    ${ARGN}                              # arguments of the function to parse (ARGN contains all the arguments after the function name)
  )
  set(ROBOCIN_EMBEDDED_CROSS_COMPILING TRUE CACHE INTERNAL "")

  if (NOT ARG_TARGET)
    message(FATAL_ERROR "robocin_mbed_setup: TARGET is required.")
  endif ()
  set(TARGET ${ARG_TARGET})
  string(TOUPPER ${TARGET} TARGET)

  set(BUILD_TYPE ${CMAKE_BUILD_TYPE})
  if (ARG_BUILD_TYPE)
    set(BUILD_TYPE ${ARG_BUILD_TYPE})
  endif ()
  string(TOLOWER ${BUILD_TYPE} BUILD_TYPE)

  if (NOT ARG_TOOLCHAIN)
    message(FATAL_ERROR "robocin_mbed_setup: TOOLCHAIN is required.")
  endif ()
  set(TOOLCHAIN ${ARG_TOOLCHAIN})
  string(TOUPPER ${TOOLCHAIN} TOOLCHAIN)

  set(MBED_BUILD_CONFIG_CMAKE_FILE "mbed_config.cmake")
  set(MBED_TOOLS_CLI "mbed-tools")

  if (NOT EXISTS "${CMAKE_CURRENT_BINARY_DIR}/${MBED_BUILD_CONFIG_CMAKE_FILE}")
    message(STATUS "Generating required Mbed OS files...")
    execute_process(
      WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
      COMMAND ${MBED_TOOLS_CLI} deploy
      OUTPUT_QUIET
    )

    execute_process(
      WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
      COMMAND ${MBED_TOOLS_CLI} configure -m ${TARGET} -b ${BUILD_TYPE} -t ${TOOLCHAIN} -o ${CMAKE_CURRENT_BINARY_DIR}
      OUTPUT_QUIET
    )
    message(STATUS "Mbed OS files generated successfully.")
  endif ()

  set(MBED_PATH ${CMAKE_CURRENT_SOURCE_DIR}/mbed-os CACHE INTERNAL "")
  set(MBED_CONFIG_PATH ${CMAKE_CURRENT_BINARY_DIR} CACHE INTERNAL "")

  include(${MBED_PATH}/tools/cmake/app.cmake)

  add_subdirectory(${MBED_PATH})

  message(STATUS "Mbed OS configured: ${TARGET} - ${BUILD_TYPE} - ${TOOLCHAIN}")
endmacro()

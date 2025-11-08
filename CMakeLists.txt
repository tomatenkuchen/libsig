###############################################################################
# cmake variables
cmake_minimum_required(VERSION 4.1)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

###############################################################################
# project definition
project(${CONAN_PROJECT_NAME}
        VERSION ${CONAN_PROJECT_VERSION}
        DESCRIPTION ${CONAN_PROJECT_DESCRIPTION}
        LANGUAGES CXX
)

###############################################################################
# find dependencies
find_program(CMAKE_CXX_CPPCHECK cppcheck REQUIRED)
find_package(mp-units REQUIRED)

# cppcheck configuration
list(
    APPEND CMAKE_CXX_CPPCHECK
      "--enable=all"
      "-cppcheck-build-dir=${CMAKE_BINARY_DIR}"
)

###############################################################################
# config target
add_library(${CONAN_PROJECT_NAME} INTERFACE )

target_include_directories(${CONAN_PROJECT_NAME}
  INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_features(${CONAN_PROJECT_NAME}
  INTERFACE
    cxx_std_23
)

# define public headers. only those are in the installation package
file(GLOB header_files "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")
set_target_properties(${CONAN_PROJECT_NAME}
  PROPERTIES
    PUBLIC_HEADER "${header_files}"
)

###############################################################################
# installer

install(TARGETS ${CONAN_PROJECT_NAME})

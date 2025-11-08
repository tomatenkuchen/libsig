###############################################################################
# cmake variables
cmake_minimum_required(VERSION 4.1)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

###############################################################################
# find dependencies
project(PackageTest CXX)

###############################################################################
# find dependencies
find_program(CMAKE_CXX_CPPCHECK cppcheck REQUIRED)
find_package(libsig CONFIG REQUIRED)
find_package(Catch2 REQUIRED)

# cppcheck configuration
list(
    APPEND CMAKE_CXX_CPPCHECK
      "--enable=all"
      "--cppcheck-build-dir=${CMAKE_BINARY_DIR}"
)

###############################################################################
# config target
add_executable(tests
  src/test_controller_pid.cpp
  src/test_filter_moving_average.cpp
  src/test_filter_moving_average.cpp
)

target_compile_features(tests
  PRIVATE
    cxx_std_23
)

target_link_libraries(tests
  PRIVATE
    libsig::libsig
    Catch2::Catch2WithMain
)

###############################################################################
# tests
include(CTest)
add_test(NAME tests COMMAND tests)

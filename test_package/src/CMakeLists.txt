cmake_minimum_required(VERSION 3.23...3.50)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

project(test_libsig
  LANGUAGES CXX
)

list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})

find_package(Catch2 REQUIRED)
find_package(mp-units REQUIRED)

include(CTest)

add_executable(tests)
target_sources(tests 
 PRIVATE
  test_controller_pid.cpp
  test_filter_moving_average.cpp
  test_power_analysis.cpp
)

target_link_libraries(tests 
 PRIVATE 
  Catch2::Catch2WithMain
  mp-units::mp-units
)

target_include_directories(tests
 PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

target_compile_features(tests 
 PRIVATE 
  cxx_std_20
)

add_test(NAME tests COMMAND tests)

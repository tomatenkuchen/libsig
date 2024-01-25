cmake_minimum_required(VERSION 3.28)

set(CMAKE_COMPILE_COMMANDS ON)

project(libsig 
  VERSION ${CONAN_PROJECT_VERSION}
  DESCRIPTION ${CONAN_PROJECT_DESCRIPTION}
  LANGUAGES CXX
  )

add_library(libsig src/pid-controller.cpp)
target_include_directories(libsig PUBLIC include)
target_compile_features(libsig PRIVATE cxx_std_20)
target_compile_options(libsig PRIVATE -Wall -Wextra -Wpedantic)

set_target_properties(libsig PROPERTIES PUBLIC_HEADER "include/libsig.hpp")
install(TARGETS libsig)

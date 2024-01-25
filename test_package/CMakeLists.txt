cmake_minimum_required(VERSION 3.28)
project(PackageTest CXX)

find_package(libsig CONFIG REQUIRED)

add_executable(example src/example.cpp)
target_link_libraries(example libsig::libsig)

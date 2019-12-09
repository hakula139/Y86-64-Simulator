# ./CMakeLists.txt

cmake_minimum_required(VERSION 2.8)

project(Y86-64-Simulator)
set(Y86-64-Simulator_VERSION_MAJOR 3)
set(Y86-64-Simulator_VERSION_MINOR 0)
set(Y86-64-Simulator_VERSION_PATCH 4)

set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

configure_file("config.h.in" "../src/config.h")

add_subdirectory(src)

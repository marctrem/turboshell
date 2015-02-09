cmake_minimum_required(VERSION 2.8.4)
project(turboshell)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES src/main.cpp src/Shell.cpp)
add_executable(turboshell ${SOURCE_FILES})
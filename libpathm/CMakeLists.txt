cmake_minimum_required(VERSION 2.8.4)
project(pathm)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(Boost 1.41.0 COMPONENTS system REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

set(SOURCE_FILES src/path.cpp)
#add_executable(libpathm ${SOURCE_FILES})
add_library(pathm ${SOURCE_FILES} include/file_util.hpp src/file_util.cpp)

target_link_libraries(pathm ${Boost_LIBRARIES})

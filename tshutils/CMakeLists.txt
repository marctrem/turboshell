cmake_minimum_required(VERSION 2.8.4)
project(tshlist)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
find_package(Boost 1.45.0 COMPONENTS system REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

set(SOURCE_FILES src/main.cpp ../libpathm/src/path.cpp)
add_executable(list ${SOURCE_FILES})
target_link_libraries(list ${Boost_LIBRARIES})
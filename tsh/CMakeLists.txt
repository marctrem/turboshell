set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

cmake_minimum_required(VERSION 2.8.4)
project(turboshell)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++14")
find_package(Boost 1.57.0 COMPONENTS system filesystem REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})

set(SOURCE_FILES src/main.cpp src/Shell.cpp ../libpathm/src/path.cpp)
add_executable(turboshell ${SOURCE_FILES})

#target_link_libraries(turboshell ${CMAKE_SOURCE_DIR}/../build/lib/libpathm.a)
target_link_libraries(turboshell ${Boost_LIBRARIES})

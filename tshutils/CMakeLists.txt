cmake_minimum_required(VERSION 2.8.4)
project(tshlist)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")
find_package(Boost 1.41.0 COMPONENTS system REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

set(SOURCE_FILES_list src/list.cpp ../libpathm/src/path.cpp)
add_executable(list ${SOURCE_FILES_list})
target_link_libraries(list ${Boost_LIBRARIES})

set(SOURCE_FILES_new src/new.cpp ../libpathm/src/path.cpp)
add_executable(new ${SOURCE_FILES_new})
target_link_libraries(new ${Boost_LIBRARIES})

set(SOURCE_FILES_newdir src/newdir.cpp ../libpathm/src/path.cpp)
add_executable(newdir ${SOURCE_FILES_newdir})
target_link_libraries(newdir ${Boost_LIBRARIES})

set(SOURCE_FILES_size src/size.cpp ../libpathm/src/path.cpp)
add_executable(size ${SOURCE_FILES_size})
target_link_libraries(size ${Boost_LIBRARIES})

set(SOURCE_FILES_rmall src/rmall.cpp ../libpathm/src/path.cpp)
add_executable(rmall ${SOURCE_FILES_rmall})
target_link_libraries(rmall ${Boost_LIBRARIES})

set(SOURCE_FILES_fin src/fin.cpp ../libpathm/src/path.cpp ../libpathm/src/file_util.cpp)
add_executable(fin ${SOURCE_FILES_fin})
target_link_libraries(fin ${Boost_LIBRARIES})

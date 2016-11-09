cmake_minimum_required(VERSION 3.6)
project(test_glog)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)
add_executable(test_glog ${SOURCE_FILES})

target_link_libraries(test_glog
        glog)
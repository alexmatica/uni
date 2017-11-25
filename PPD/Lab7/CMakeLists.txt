cmake_minimum_required(VERSION 3.8)
project(Lab7)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
set(SOURCE_FILES main.cpp)
add_executable(Lab7 ${SOURCE_FILES})
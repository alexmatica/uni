cmake_minimum_required(VERSION 3.8)
project(Project)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp my_png.h my_png.cpp)
add_executable(Project ${SOURCE_FILES})
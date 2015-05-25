cmake_minimum_required(VERSION 3.1)
project(Mirror)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    Mirror/head.cpp)

add_executable(Mirror ${SOURCE_FILES})
cmake_minimum_required(VERSION 3.6)
project(SO_RiverCrossingProblem)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.c)
add_executable(SO_RiverCrossingProblem ${SOURCE_FILES})
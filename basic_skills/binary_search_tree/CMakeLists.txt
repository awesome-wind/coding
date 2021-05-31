cmake_minimum_required(VERSION 3.5)
project(binary_tree)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES binary_tree.cpp SequenceST.h FileOps.h)
add_executable(binary_tree ${SOURCE_FILES})
cmake_minimum_required(VERSION 3.10)

project(graph)

set(CMAKE_CXX_FLAGS "CMAKE_CXX_FLAGS -std=c++14")

set(SOURCES main.cpp dense_graph.h sparse_graph.h)

add_executable(graph ${SOURCES})
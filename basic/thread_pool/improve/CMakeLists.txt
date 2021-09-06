cmake_minimum_required(VERSION 3.5)
project(thread_pool)

include_directories(${CMAKE_CURRENT_LIST_DIR})

set(HEADERS safe_queue.h thread_pool.h)

set(SOURCES main.cpp)

set(CMAKE_CXX_FLAGS -pthread)

add_compile_options(-std=c++14)

add_executable(main ${HEADERS} ${SOURCES})
target_link_libraries(main)
cmake_minimum_required(VERSION 3.10.0)

project(gtest VERSION 1.0)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-std=c++14")
set(CMAKE_EXE_LINKER_FLAGS "-lpthread")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(PROJECT_PATH "${CMAKE_CURRENT_LIST_DIR}")

include("${PROJECT_PATH}/cmake/config.cmake")

find_package(GTest REQUIRED CONFIG)

include_directories("${PROJECT_PATH}/include")

set(src_files
    main.cpp 
    skills_test.cpp
    leetcode_test.cpp
    binary_tree/binary_tree.cpp 
    thread_pool/basic/thread_pool.cpp
    graph/graph.cpp
    leetcode/is_match.cpp
    leetcode/merge_lists.cpp
    leetcode/tools.cpp
)

add_executable(main ${src_files})

target_link_libraries(main GTest::gtest GTest::gmock_main)

target_include_directories(main PRIVATE "${PROJECT_PATH}" "${PROJECT_PATH}/gtest/include")


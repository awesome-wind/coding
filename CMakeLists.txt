cmake_minimum_required(VERSION 3.10.0)

project(gtest VERSION 1.0)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-std=c++14 -mmacosx-version-min=10.10")
set(CMAKE_EXE_LINKER_FLAGS "-lpthread")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(PROJECT_PATH "${CMAKE_CURRENT_LIST_DIR}")

include("${PROJECT_PATH}/cmake/config.cmake")

find_package(GTest REQUIRED CONFIG)

include_directories("${PROJECT_PATH}/include")

set(src_files
    main.cpp 
    basic_test.cpp
    leetcode_test.cpp
    
    basic/binary_tree/binary_tree.cpp 
    basic/thread_pool/basic/thread_pool.cpp
    basic/graph/graph.cpp
    basic/base/async_access.cpp

    leetcode/is_match.cpp
    leetcode/merge_lists.cpp
    leetcode/tools.cpp
    leetcode/permute.cpp
    leetcode/rotate.cpp
)

add_executable(main_test ${src_files})

target_link_libraries(main_test GTest::gtest GTest::gmock_main)

target_include_directories(main_test PRIVATE "${PROJECT_PATH}" "${PROJECT_PATH}/basic" "${PROJECT_PATH}/gtest/include")


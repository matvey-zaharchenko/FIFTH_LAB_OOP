cmake_minimum_required(VERSION 3.10)
project(lab05)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(GTest REQUIRED)

add_library(${CMAKE_PROJECT_NAME}_lib INTERFACE
    include/customMemoryRes.h
    include/stackIterator.h
    include/stack.h
)

target_include_directories(${CMAKE_PROJECT_NAME}_lib INTERFACE include)

add_executable(${CMAKE_PROJECT_NAME}_out main.cpp)
target_link_libraries(${CMAKE_PROJECT_NAME}_out PRIVATE ${CMAKE_PROJECT_NAME}_lib)

enable_testing()

add_executable(tests tests/stack_test.cpp)
target_link_libraries(tests PRIVATE ${CMAKE_PROJECT_NAME}_lib GTest::gtest GTest::gtest_main)

target_include_directories(tests PRIVATE include)

add_test(NAME StackTest COMMAND tests)
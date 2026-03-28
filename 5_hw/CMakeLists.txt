cmake_minimum_required(VERSION 3.28)
project(5_hw)

set(CMAKE_CXX_STANDARD 23)

add_subdirectory(../4_hw 4_hw_build)

file(GLOB_RECURSE ALL_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
)

add_executable(5_hw main.cpp
        PolyDivision.h)

target_include_directories(5_hw PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/Ordering
)

target_link_libraries(5_hw PRIVATE hw4_lib)
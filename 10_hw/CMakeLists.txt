cmake_minimum_required(VERSION 3.28)
project(10_hw)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SOURCES "big_float/src/*.cpp")

add_executable(10_hw main.cpp ${SOURCES})

target_include_directories(10_hw PRIVATE big_float/inc)
cmake_minimum_required(VERSION 3.28)
project(7_hw)

set(CMAKE_CXX_STANDARD 17)

file(GLOB PROJECT_SOURCES *.cpp *.hpp)

add_subdirectory(../5_hw 5_hw_build)

add_executable(7_hw ${PROJECT_SOURCES})

target_include_directories(7_hw PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(7_hw PRIVATE hw5_lib)
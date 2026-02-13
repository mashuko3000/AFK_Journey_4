cmake_minimum_required(VERSION 3.28)
project(crypt_1)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/heads/main.zip
)

FetchContent_MakeAvailable(googletest)

add_executable(crypt_1 main.cpp
        inc/test.hpp
        src/p_box.cpp
        inc/p_box.hpp
        inc/common.hpp
        src/s_box.cpp
        inc/s_box.hpp
        tests/test2.cpp)
add_executable(unit_tests
        tests/test1.cpp
        tests/test2.cpp
        src/s_box.cpp
        src/p_box.cpp)
target_link_libraries(unit_tests gtest_main)
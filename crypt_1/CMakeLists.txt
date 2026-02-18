cmake_minimum_required(VERSION 3.28)
project(crypt_1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/heads/main.zip
)

FetchContent_MakeAvailable(googletest)

set(COMMON_SOURCES
        cl_sr/src/rc4_cipher.cpp
        cl_sr/src/ipc_manager.cpp
)

set(COMMON_INCLUDES
        cl_sr/inc
)

add_executable(server
        cl_sr/apps/server/server.cpp
        ${COMMON_SOURCES}
)

target_include_directories(server PRIVATE ${COMMON_INCLUDES})

add_executable(client
        cl_sr/apps/client/client.cpp
        ${COMMON_SOURCES}
)

target_include_directories(client PRIVATE ${COMMON_INCLUDES})

add_executable(crypt_1 main.cpp
        inc/test.hpp
        src/p_box.cpp
        inc/p_box.hpp
        inc/common.hpp
        src/s_box.cpp
        inc/s_box.hpp
        tests/test2.cpp
        inc/loop.hpp
        src/loop.cpp
        tests/test3.cpp)
add_executable(unit_tests
        tests/test1.cpp
        tests/test2.cpp
        src/s_box.cpp
        src/p_box.cpp
        src/loop.cpp
        tests/test3.cpp)
target_link_libraries(unit_tests gtest_main)

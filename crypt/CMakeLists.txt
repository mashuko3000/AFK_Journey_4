cmake_minimum_required(VERSION 3.28)
project(crypt)

set(CMAKE_CXX_STANDARD 17)

find_package(Boost REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/inc)

file(GLOB_RECURSE CRYPTO_SOURCES
        "src/*.cpp"
        "src/*.hpp"
        "inc/*.hpp"
        "inc/*.h"
)

list(FILTER CRYPTO_SOURCES EXCLUDE REGEX "main.cpp")

add_library(crypto_lib STATIC ${CRYPTO_SOURCES})

add_executable(crypt_app app/main.cpp
        tests/test_main.cpp
        inc/service/CipherContext.hpp
        src/service/CipherContext.cpp)

target_link_libraries(crypt_app PRIVATE crypto_lib)

target_include_directories(crypt_app PRIVATE
        ${Boost_INCLUDE_DIRS}
)

enable_testing()
find_package(GTest REQUIRED)
add_executable(crypto_tests tests/test_main.cpp)
target_link_libraries(crypto_tests PRIVATE crypto_lib GTest::gtest_main)


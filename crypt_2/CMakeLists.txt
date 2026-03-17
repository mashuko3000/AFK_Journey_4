cmake_minimum_required(VERSION 3.28)
project(crypt_2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED COMPONENTS system)

file(GLOB_RECURSE LIB_SOURCES "src/*.cpp" "padding/*.cpp" "mode/*.cpp" "context/*.cpp" "helpers/*.cpp")
add_library(crypto_lib STATIC ${LIB_SOURCES})
target_include_directories(crypto_lib PUBLIC inc crypt_1/inc core_interfaces padding mode context helpers)

include(FetchContent)
FetchContent_Declare(googletest URL https://github.com/google/googletest/archive/refs/heads/main.zip)
FetchContent_Declare(bigmath GIT_REPOSITORY https://github.com/koftamainee/bigmath.git GIT_TAG master)

set(BIGMATH_BUILD_TESTS OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest bigmath)

add_executable(crypt_2 main.cpp
        core_interfaces/i_primary_test.hpp
        primality_tests/BasePrimalityTest.hpp
        primality_tests/FermatPrimalityTest.hpp)
target_link_libraries(crypt_2 PRIVATE crypto_lib bigmath++ Boost::system)

add_executable(crypto_unit_tests tests.cpp unit_tests/unit_tests_padding_and_mode.cpp)
target_link_libraries(crypto_unit_tests PRIVATE crypto_lib GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(crypto_unit_tests)
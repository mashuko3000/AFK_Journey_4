cmake_minimum_required(VERSION 3.28)
project(crypt_2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE LIB_SOURCES "src/*.cpp" "crypt_1/src/*.cpp")
add_library(crypto_lib STATIC ${LIB_SOURCES})
target_include_directories(crypto_lib PUBLIC inc crypt_1/inc)

add_executable(crypt_2 main.cpp)
target_link_libraries(crypt_2 PRIVATE crypto_lib)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/heads/main.zip
)

FetchContent_MakeAvailable(googletest)

add_executable(crypto_unit_tests tests.cpp)
target_link_libraries(crypto_unit_tests GTest::gtest_main crypto_lib m pthread)

include(GoogleTest)
gtest_discover_tests(crypto_unit_tests)
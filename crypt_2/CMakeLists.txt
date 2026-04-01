cmake_minimum_required(VERSION 3.28)
project(crypt_2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED COMPONENTS system)

file(GLOB_RECURSE LIB_SOURCES
        "src/*.cpp" "padding/*.cpp" "mode/*.cpp" "context/*.cpp"
        "helpers/*.cpp" "RSA/*.cpp" "service/*.cpp"  "attack/rsa_attack/*.cpp" "boxes/*.cpp"
)
add_library(crypto_lib STATIC ${LIB_SOURCES}
        helpers/FractionService.cpp
        helpers/FractionService.h)
target_include_directories(crypto_lib PUBLIC
        des_inc attack/rsa_attack boxes core_interfaces padding mode context helpers RSA service primality_tests
)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
FetchContent_MakeAvailable(googletest)

add_executable(crypt_2 main.cpp)
target_link_libraries(crypt_2 PRIVATE crypto_lib Boost::boost Boost::system)

enable_testing()
include(GoogleTest)

file(GLOB_RECURSE TEST_SOURCES "unit_tests/*.cpp")

if(TEST_SOURCES)
    add_executable(unit_tests ${TEST_SOURCES}
            unit_tests/FermatAttackTests.cpp
            unit_tests/WienerAttackTest.cpp
            unit_tests/HelpersTests.cpp
    )
    target_link_libraries(unit_tests
            PRIVATE
            crypto_lib
            Boost::boost
            Boost::system
            gtest_main
            gtest
    )
    gtest_discover_tests(unit_tests)
endif()
cmake_minimum_required(VERSION 3.28)
project(crypt_2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE LIB_SOURCES "src/*.cpp" "crypt_1/src/*.cpp")
add_library(crypto_lib STATIC ${LIB_SOURCES})
target_include_directories(crypto_lib PUBLIC inc crypt_1/inc)

add_executable(crypt_2 main.cpp
        "core_interfaces/crypto_enums.hpp"
        "core_interfaces/i_padding.hpp"
        "core_interfaces/i_mode.hpp"
        padding/zeros_padding.cpp
        padding/zeros_padding.hpp
        padding/ansi_padding.cpp
        padding/ansi_padding.hpp
        padding/pcks7_padding.cpp
        padding/iso10126_padding.cpp
        mode/ECB_mode.cpp
        mode/ECB_mode.hpp
        mode/CBC_mode.cpp
        mode/CBC_mode.hpp
        mode/PCBC_mode.cpp
        mode/PCBC_mode.hpp
        context/symmetric_context.cpp
        context/symmetric_context.hpp
        "context/cipher_context.hpp"
        context/cipher_context.cpp
        unit_tests/unit_tests_padding_and_mode.cpp)

target_link_libraries(crypt_2 PRIVATE crypto_lib)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/heads/main.zip
)

FetchContent_MakeAvailable(googletest)

set(COMMON_SOURCES
        padding/zeros_padding.cpp
        padding/ansi_padding.cpp
        mode/ECB_mode.cpp
        mode/CBC_mode.cpp
        mode/PCBC_mode.cpp
        context/cipher_context.cpp
        context/symmetric_context.cpp
        padding/pcks7_padding.cpp
        padding/pcks7_padding.hpp
        padding/iso10126_padding.cpp
        padding/iso10126_padding.hpp
        mode/CFB_mode.cpp
        mode/CFB_mode.hpp
        mode/OFB_mode.cpp
        mode/OFB_mode.hpp
        mode/CTR_mode.cpp
        mode/CTR_mode.hpp
        mode/RandomDelta.cpp
        mode/RandomDelta.hpp
)


add_executable(crypto_unit_tests tests.cpp ${COMMON_SOURCES})
target_link_libraries(crypto_unit_tests PRIVATE GTest::gtest_main crypto_lib)

include(GoogleTest)
gtest_discover_tests(crypto_unit_tests)
cmake_minimum_required(VERSION 3.28)
project(crypt)

set(CMAKE_CXX_STANDARD 17)

find_package(Boost REQUIRED COMPONENTS system)
find_package(GTest REQUIRED)
find_package(OpenSSL REQUIRED)

file(GLOB_RECURSE CRYPTO_SOURCES
        "src/*.cpp"
        "src/*.c"
)

add_library(crypto_lib STATIC ${CRYPTO_SOURCES})

target_include_directories(crypto_lib PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/inc
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${OpenSSL_INCLUDE_DIR}
        ${Boost_INCLUDE_DIRS}
)

target_link_libraries(crypto_lib PUBLIC
        OpenSSL::Crypto
        Boost::system
)

add_executable(crypt_app app/main.cpp)
target_link_libraries(crypt_app PRIVATE crypto_lib)

enable_testing()
add_executable(crypto_tests
        tests/test_main.cpp
        tests/AliceBob.cpp
)
target_link_libraries(crypto_tests PRIVATE
        crypto_lib
        GTest::gtest_main
)
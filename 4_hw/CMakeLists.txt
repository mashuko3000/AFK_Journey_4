cmake_minimum_required(VERSION 3.28)
project(4_hw)

set(CMAKE_CXX_STANDARD 17)

add_executable(4_hw main.cpp
        inc/Polynimial.hpp
        inc/Monomial.hpp
        inc/TrieNode.hpp
        inc/ZMod.hpp)

add_library(hw4_lib INTERFACE)

target_include_directories(hw4_lib INTERFACE inc)

target_sources(hw4_lib INTERFACE
        inc/Polynimial.hpp
        inc/Monomial.hpp
        inc/TrieNode.hpp
        inc/ZMod.hpp
)
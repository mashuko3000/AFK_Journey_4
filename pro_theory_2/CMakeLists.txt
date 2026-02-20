cmake_minimum_required(VERSION 3.28)
project(pro_theory)

set(CMAKE_CXX_STANDARD 17)

add_executable(pro_theory main.cpp
        gmurman/gmurman.hpp
        gmurman/gmurman.cpp)

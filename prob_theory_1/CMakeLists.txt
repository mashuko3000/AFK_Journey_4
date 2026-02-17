cmake_minimum_required(VERSION 3.28)
project(prob_theory_1)

set(CMAKE_CXX_STANDARD 17)

add_executable(prob_theory_1 main.cpp
        gmurman/src/gmurman.cpp
        gmurman/inc/gmurman.hpp
        gmurman/inc/gmurman.hpp)

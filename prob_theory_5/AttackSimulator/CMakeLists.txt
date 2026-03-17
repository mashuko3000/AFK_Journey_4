cmake_minimum_required(VERSION 3.28)
project(AttackSimulator)

set(CMAKE_CXX_STANDARD 17)

add_executable(AttackSimulator main.cpp
        interfaces.hpp
        VernamAttack.hpp)

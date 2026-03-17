cmake_minimum_required(VERSION 3.28)
project(CipherAttack)

set(CMAKE_CXX_STANDARD 17)

add_executable(CipherAttack main.cpp
        interfaces.hpp
        VernamCipher.hpp
        VigenereCipher.hpp
        types.hpp
        StatisticalAttack.hpp
        Attack.hpp
)

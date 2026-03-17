cmake_minimum_required(VERSION 3.28)
project(Email)

set(CMAKE_CXX_STANDARD 17)

add_executable(Email main.cpp
        interfacesAndEntity.hpp
        Memory.hpp
        NLP.hpp
        NaiveBayes.hpp
        StandardEvaluator.hpp
        PipeLine.hpp
        runApp.hpp)

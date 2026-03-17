cmake_minimum_required(VERSION 3.28)
project(Family)

set(CMAKE_CXX_STANDARD 17)

add_executable(Family main.cpp
        interfaces.hpp
        types.hpp
        "../types.hpp"
        "../interfaces.hpp"
        math/idistribution.hpp
        math/distribution.hpp
        PoissonFamilyCalculator/PoissonFamilyCalculator.cpp
        PoissonFamilyCalculator/PoissonFamilyCalculator.hpp
        PoissonFamilySimulator/PoissonFamilySimulator.cpp
        PoissonFamilySimulator/PoissonFamilySimulator.hpp
        EmpiricalAnalyzer/EmpiricalAnalyzer.cpp
        EmpiricalAnalyzer/EmpiricalAnalyzer.hpp)

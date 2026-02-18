cmake_minimum_required(VERSION 3.28)
project(prob_theory_1)

set(CMAKE_CXX_STANDARD 17)

add_executable(task_1
        part_2/1_task/1_task.hpp
        part_2/1_task/1_task.cpp
        p2_1.cpp)

add_executable(task_2
        part_2/2_task/2_task.hpp
        part_2/2_task/2_task.cpp
        p2_2.cpp)

add_executable(gmurman
        main.cpp
        gmurman/src/gmurman.cpp
        gmurman/inc/gmurman.hpp
        part_2/2_task/2_task.cpp)

add_executable(task_3
        p2_3.cpp)
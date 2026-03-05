cmake_minimum_required(VERSION 3.28)
project(prob_theory_3_3)

set(CMAKE_CXX_STANDARD 17)

set(SOURCES
        main.cpp
        core/generator/random_generator.cpp
        models/components/components.cpp
        models/recipe/recipe.cpp
        models/inventory/inventory.cpp
        logic/quality_logic/quality_logic.cpp
        logic/stats_logic/stats_logic.cpp
        infrastructure/cli_parser.cpp
        machines/base_machine/base_machine.cpp
        machines/assembley_machine/assembley_machine.cpp
        machines/recycle_machine/recycle_machine.cpp
        factory/factory.cpp
        factory/factory.hpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/core
        ${CMAKE_CURRENT_SOURCE_DIR}/models
        ${CMAKE_CURRENT_SOURCE_DIR}/logic
        ${CMAKE_CURRENT_SOURCE_DIR}/infrastructure
        ${CMAKE_CURRENT_SOURCE_DIR}/machines
)

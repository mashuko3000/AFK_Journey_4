cmake_minimum_required(VERSION 3.28)
project(prob_theory_6)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CORE_SOURCES
        Core/IExperiment.hpp
        Core/RandomGenerator.hpp
        Core/MathUtilits.hpp
        Barrel/BarrelExplosion.hpp
        Coin/Coin.hpp
        Gmurman/Gmurman.hpp
)

add_library(prob_core OBJECT ${CORE_SOURCES})

add_executable(prob_theory_console
        main_console.cpp
        $<TARGET_OBJECTS:prob_core>
)

set(GUI_SOURCES
        Simulation/Widget.hpp
        Simulation/Simulation.hpp
        Simulation/MainWindow.hpp
        Simulation/MainWindow.cpp
)

add_executable(prob_theory_gui
        main_gui.cpp
        ${GUI_SOURCES}
        $<TARGET_OBJECTS:prob_core>
)

target_link_libraries(prob_theory_gui PRIVATE Qt6::Widgets)

set_target_properties(prob_theory_console PROPERTIES
        OUTPUT_NAME "prob_theory_console"
)

set_target_properties(prob_theory_gui PROPERTIES
        OUTPUT_NAME "prob_theory_gui"
)


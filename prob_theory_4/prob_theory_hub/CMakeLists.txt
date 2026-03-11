cmake_minimum_required(VERSION 3.28)
project(prob_theory_hub)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        PrintSupport
        REQUIRED)

set(QCUSTOMPLOT_SOURCES
        external/qcustomplot.cpp
        external/qcustomplot.h
        math/idistribution.hpp
        math/distribution.hpp
        drunk_man/DrunkardEngine.cpp
        drunk_man/DrunkardEngine.hpp
        drunk_man/DrankManWindow.cpp
        drunk_man/DrankManWindow.hpp
        point/RandomWalking.cpp
        point/RandomWalking.hpp
        point/HistoryLogger.hpp
        point/Config.hpp
        point/Window.cpp
        point/Window.hpp
        rw_return_to_origin/PontWalking.cpp
        rw_return_to_origin/PontWalking.hpp
        complex_random_walking/WalkEngine.cpp
        complex_random_walking/WalkEngine.hpp
        complex_random_walking/ConfigParser.hpp
        complex_random_walking/ComplexWindow.cpp
        complex_random_walking/ComplexWindow.hpp
        stairs/StaircaseSimulator.cpp
        stairs/StaircaseSimulator.hpp
        stairs/ConfigLoader.hpp
        stairs/StairsWindow.cpp
        stairs/StairsWindow.hpp
)

add_executable(prob_theory_hub main.cpp
        base_ui/UniversalBaseWindow.cpp
        base_ui/UniversalBaseWindow.hpp
        base_ui/base_enums.hpp
        ${QCUSTOMPLOT_SOURCES}
)

target_include_directories(prob_theory_hub PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/external
)

target_link_libraries(prob_theory_hub
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt6::PrintSupport
)

if (WIN32 AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(DEBUG_SUFFIX)
    if (MSVC AND CMAKE_BUILD_TYPE MATCHES "Debug")
        set(DEBUG_SUFFIX "d")
    endif ()
    set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
    if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
        set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        endif ()
    endif ()
    if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
    endif ()
    foreach (QT_LIB Core Gui Widgets)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/bin/Qt6${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach (QT_LIB)
endif ()

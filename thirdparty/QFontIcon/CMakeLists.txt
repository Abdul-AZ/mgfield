# Generated from QFontIcon.pri.

cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 17)
project(QFontIcon VERSION 1.0 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
    set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}")

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Gui)

qt_add_library(QFontIcon
    qfonticon.cpp qfonticon.h
)
target_include_directories(QFontIcon PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(QFontIcon PUBLIC
    Qt::Core
    Qt::Gui
    Qt::Widgets
)

install(TARGETS QFontIcon
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)

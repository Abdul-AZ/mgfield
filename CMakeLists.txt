# Generated from mgfield.pro.

cmake_minimum_required(VERSION 3.25.3)
set(CMAKE_CXX_STANDARD 17)
project(mgfield VERSION 1.0 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
    set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}")

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Gui)
find_package(Qt${QT_VERSION_MAJOR} OPTIONAL_COMPONENTS OpenGL OpenGLWidgets Widgets)

set(SOURCES "")
set(LIBS "")
set(INCLUDE_DIRS "")

qt_add_executable(mgfield WIN32 MACOSX_BUNDLE
)

add_subdirectory(src)
add_subdirectory(thirdparty)

target_sources(mgfield PUBLIC ${SOURCES})


target_include_directories(mgfield PRIVATE thirdparty ${INCLUDE_DIRS})

target_link_libraries(mgfield PUBLIC
    Qt::Core
    Qt::Gui
    Qt::OpenGL
    Qt::OpenGLWidgets
    Qt::Widgets
    libheatmap
    ${LIBS}
)


# Resources:
set(application_resource_files
    "res/data/magnet_models.json"
    "res/foundation-icons.ttf"
    "res/icon_256px.png"
    "res/shapes/Arrow3D.glb"
    "res/shapes/Cable3D.glb"
    "res/shapes/Plane3D.glb"
    "shaders/ColoredMesh.frag"
    "shaders/ColoredMesh.vert"
    "shaders/VectorField3D.frag"
    "shaders/VectorField3D.vert"
)

qt6_add_resources(mgfield "application"
    PREFIX
        "/"
    FILES
        ${application_resource_files}
)


if(WIN32)
    target_link_libraries(mgfield PUBLIC
        opengl32
    )
endif()

install(TARGETS mgfield
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)

# Generated from mgfield.pro.

cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 17)
project(mgfield VERSION 1.0 LANGUAGES CXX)

if(MSVC)
    target_compile_options(qt_cmake PUBLIC "/Zc:__cplusplus")
endif()

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

qt_add_executable(mgfield WIN32 MACOSX_BUNDLE
    src/freeroamcameracontroller.cpp src/freeroamcameracontroller.h
    src/grid3d.cpp src/grid3d.h
    src/main.cpp
    src/mainwindow.cpp src/mainwindow.h src/mainwindow.ui
    src/modelloader.cpp src/modelloader.h
    src/objectitemdelegate.cpp src/objectitemdelegate.h
    src/objectitemmodel.cpp src/objectitemmodel.h
    src/orbitcameracontroller.cpp src/orbitcameracontroller.h
    src/perspectivecamera.cpp src/perspectivecamera.h
    src/scene.cpp src/scene.h
    src/scene/currentcarryingsheet.cpp src/scene/currentcarryingsheet.h
    src/scene/object.h
    src/scene/objectrenderer.cpp src/scene/objectrenderer.h
    src/scene/permanentmagnet.cpp src/scene/permanentmagnet.h
    src/scene/straightwireobject.cpp src/scene/straightwireobject.h
    src/sim/mfsimulator.cpp src/sim/mfsimulator.h
    src/sim/simulationresultsexporter.cpp src/sim/simulationresultsexporter.h
    src/vectorfield3d.cpp src/vectorfield3d.h
    src/view/addobjectdialog.cpp src/view/addobjectdialog.h src/view/addobjectdialog.ui
    src/view/modifieddoublespinbox.cpp src/view/modifieddoublespinbox.h
    src/view/objectinspector.cpp src/view/objectinspector.h src/view/objectinspector.ui
    src/view/objectlistview.cpp src/view/objectlistview.h
    src/view/simulationsettingsdialog.cpp src/view/simulationsettingsdialog.h src/view/simulationsettingsdialog.ui
    src/view/viewport3d.cpp src/view/viewport3d.h
    src/view/viewportsettings.cpp src/view/viewportsettings.h src/view/viewportsettings.ui
    thirdparty/eng_format/eng_format.cpp thirdparty/eng_format/eng_format.hpp
    thirdparty/tinygltf/json.hpp
    thirdparty/tinygltf/stb_image.h
    thirdparty/tinygltf/stb_image_write.h
    thirdparty/tinygltf/tiny_gltf.cpp thirdparty/tinygltf/tiny_gltf.h
)

add_subdirectory(thirdparty/QFontIcon)

target_link_libraries(mgfield PUBLIC
    Qt::Core
    Qt::Gui
    Qt::OpenGL
    Qt::OpenGLWidgets
    Qt::Widgets
    QFontIcon
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

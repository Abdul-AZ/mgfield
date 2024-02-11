QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets opengl openglwidgets

CONFIG += c++17

SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp \
    src/orbitcameracontroller.cpp \
    src/perspectivecamera.cpp \
    src/freeroamcameracontroller.cpp \
    src/grid3d.cpp \
    src/view/modifieddoublespinbox.cpp \
    src/view/objectinspector.cpp \
    src/view/objectlistview.cpp \
    src/scene.cpp \
    src/sim/mfsimulator.cpp \
    src/view/simulationsettingsdialog.cpp \
    src/view/viewportsettings.cpp \
    src/transmissioncable.cpp \
    src/vectorfield3d.cpp \
    src/view/viewport3d.cpp \
    thirdparty/tinygltf/tiny_gltf.cpp

HEADERS += \
    src/mainwindow.h \
    src/orbitcameracontroller.h \
    src/perspectivecamera.h \
    src/freeroamcameracontroller.h \
    src/grid3d.h \
    src/scene/object.h \
    src/view/modifieddoublespinbox.h \
    src/view/objectinspector.h \
    src/view/objectlistview.h \
    src/scene.h \
    src/sim/mfsimulator.h \
    src/view/simulationsettingsdialog.h \
    src/view/viewportsettings.h \
    src/transmissioncable.h \
    src/vectorfield3d.h \
    src/view/viewport3d.h \
    thirdparty/tinygltf/json.hpp \
    thirdparty/tinygltf/stb_image.h \
    thirdparty/tinygltf/stb_image_write.h \
    thirdparty/tinygltf/tiny_gltf.h

FORMS += \
    src/mainwindow.ui \
    src/view/objectinspector.ui \
    src/view/simulationsettingsdialog.ui \
    src/view/viewportsettings.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    application.qrc

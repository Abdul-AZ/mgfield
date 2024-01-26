QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets opengl openglwidgets

CONFIG += c++17

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    src/orbitcameracontroller.cpp \
    perspectivecamera.cpp \
    src/freeroamcameracontroller.cpp \
    src/grid3d.cpp \
    src/view/objectlistview.cpp \
    src/scene.cpp \
    src/sim/mfsimulator.cpp \
    src/view/simulationsettingsdialog.cpp \
    src/view/viewportsettings.cpp \
    transmissioncable.cpp \
    vectorfield3d.cpp \
    src/view/viewport3d.cpp \
    thirdparty/tinygltf/tiny_gltf.cpp

HEADERS += \
    mainwindow.h \
    src/orbitcameracontroller.h \
    perspectivecamera.h \
    src/freeroamcameracontroller.h \
    src/grid3d.h \
    src/view/objectlistview.h \
    src/scene.h \
    src/sim/mfsimulator.h \
    src/view/simulationsettingsdialog.h \
    src/view/viewportsettings.h \
    transmissioncable.h \
    vectorfield3d.h \
    src/view/viewport3d.h \
    thirdparty/tinygltf/json.hpp \
    thirdparty/tinygltf/stb_image.h \
    thirdparty/tinygltf/stb_image_write.h \
    thirdparty/tinygltf/tiny_gltf.h

FORMS += \
    mainwindow.ui \
    src/view/simulationsettingsdialog.ui \
    src/view/viewportsettings.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    application.qrc

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets opengl openglwidgets

CONFIG += c++17

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    orbitcameracontroller.cpp \
    perspectivecamera.cpp \
    src/sim/mfsimulator.cpp \
    transmissioncable.cpp \
    vectorfield3d.cpp \
    viewport3d.cpp \
    thirdparty/tinygltf/tiny_gltf.cpp

HEADERS += \
    mainwindow.h \
    orbitcameracontroller.h \
    perspectivecamera.h \
    src/sim/mfsimulator.h \
    transmissioncable.h \
    vectorfield3d.h \
    viewport3d.h \
    thirdparty/tinygltf/json.hpp \
    thirdparty/tinygltf/stb_image.h \
    thirdparty/tinygltf/stb_image_write.h \
    thirdparty/tinygltf/tiny_gltf.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    application.qrc

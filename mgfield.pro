QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets opengl openglwidgets

CONFIG += c++17
win32 {
    LIBS += -lopengl32
}

DEFINES += ENG_FORMAT_MICRO_GLYPH=\\\"u\\\"

include("thirdparty/QFontIcon/QFontIcon.pri")

SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp \
    src/modelloader.cpp \
    src/objectitemdelegate.cpp \
    src/objectitemmodel.cpp \
    src/orbitcameracontroller.cpp \
    src/perspectivecamera.cpp \
    src/freeroamcameracontroller.cpp \
    src/grid3d.cpp \
    src/scene/currentcarryingsheet.cpp \
    src/scene/objectrenderer.cpp \
    src/scene/permanentmagnet.cpp \
    src/scene/straightwireobject.cpp \
    src/view/addobjectdialog.cpp \
    src/view/modifieddoublespinbox.cpp \
    src/view/objectinspector.cpp \
    src/view/objectlistview.cpp \
    src/scene.cpp \
    src/sim/mfsimulator.cpp \
    src/view/simulationsettingsdialog.cpp \
    src/view/viewportsettings.cpp \
    src/vectorfield3d.cpp \
    src/view/viewport3d.cpp \
    thirdparty/tinygltf/tiny_gltf.cpp \
    thirdparty/eng_format/eng_format.cpp

HEADERS += \
    src/mainwindow.h \
    src/modelloader.h \
    src/objectitemdelegate.h \
    src/objectitemmodel.h \
    src/orbitcameracontroller.h \
    src/perspectivecamera.h \
    src/freeroamcameracontroller.h \
    src/grid3d.h \
    src/scene/currentcarryingsheet.h \
    src/scene/object.h \
    src/scene/objectrenderer.h \
    src/scene/permanentmagnet.h \
    src/scene/straightwireobject.h \
    src/view/addobjectdialog.h \
    src/view/modifieddoublespinbox.h \
    src/view/objectinspector.h \
    src/view/objectlistview.h \
    src/scene.h \
    src/sim/mfsimulator.h \
    src/view/simulationsettingsdialog.h \
    src/view/viewportsettings.h \
    src/vectorfield3d.h \
    src/view/viewport3d.h \
    thirdparty/tinygltf/json.hpp \
    thirdparty/tinygltf/stb_image.h \
    thirdparty/tinygltf/stb_image_write.h \
    thirdparty/tinygltf/tiny_gltf.h \
    thirdparty/eng_format/eng_format.hpp

FORMS += \
    src/mainwindow.ui \
    src/view/addobjectdialog.ui \
    src/view/objectinspector.ui \
    src/view/simulationsettingsdialog.ui \
    src/view/viewportsettings.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    application.qrc

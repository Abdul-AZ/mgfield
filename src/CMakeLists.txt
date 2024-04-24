set(SOURCES
    ${SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/freeroamcameracontroller.cpp src/freeroamcameracontroller.h
    ${CMAKE_CURRENT_SOURCE_DIR}/grid3d.cpp src/grid3d.h
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/config.h
    ${CMAKE_CURRENT_SOURCE_DIR}/mainwindow.cpp src/mainwindow.h src/mainwindow.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/modelloader.cpp src/modelloader.h
    ${CMAKE_CURRENT_SOURCE_DIR}/objectitemdelegate.cpp src/objectitemdelegate.h
    ${CMAKE_CURRENT_SOURCE_DIR}/objectitemmodel.cpp src/objectitemmodel.h
    ${CMAKE_CURRENT_SOURCE_DIR}/orbitcameracontroller.cpp src/orbitcameracontroller.h
    ${CMAKE_CURRENT_SOURCE_DIR}/perspectivecamera.cpp src/perspectivecamera.h
    ${CMAKE_CURRENT_SOURCE_DIR}/scene.cpp src/scene.h
    ${CMAKE_CURRENT_SOURCE_DIR}/scene/currentcarryingsheet.cpp src/scene/currentcarryingsheet.h
    ${CMAKE_CURRENT_SOURCE_DIR}/scene/object.h
    ${CMAKE_CURRENT_SOURCE_DIR}/scene/objectrenderer.cpp src/scene/objectrenderer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/scene/permanentmagnet.cpp src/scene/permanentmagnet.h
    ${CMAKE_CURRENT_SOURCE_DIR}/scene/straightwireobject.cpp src/scene/straightwireobject.h
    ${CMAKE_CURRENT_SOURCE_DIR}/sim/mfsimulator.cpp src/sim/mfsimulator.h
    ${CMAKE_CURRENT_SOURCE_DIR}/sim/simulationresultsexporter.cpp src/sim/simulationresultsexporter.h
    ${CMAKE_CURRENT_SOURCE_DIR}/vectorfield3d.cpp src/vectorfield3d.h
    ${CMAKE_CURRENT_SOURCE_DIR}/view/addobjectdialog.cpp src/view/addobjectdialog.h src/view/addobjectdialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/view/modifieddoublespinbox.cpp src/view/modifieddoublespinbox.h
    ${CMAKE_CURRENT_SOURCE_DIR}/view/objectinspector.cpp src/view/objectinspector.h src/view/objectinspector.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/view/objectlistview.cpp src/view/objectlistview.h
    ${CMAKE_CURRENT_SOURCE_DIR}/view/simulationsettingsdialog.cpp src/view/simulationsettingsdialog.h src/view/simulationsettingsdialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/view/viewport3d.cpp src/view/viewport3d.h
    ${CMAKE_CURRENT_SOURCE_DIR}/view/viewportsettings.cpp src/view/viewportsettings.h src/view/viewportsettings.ui
    PARENT_SCOPE
)

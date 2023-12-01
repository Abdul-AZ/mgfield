#ifndef VIEWPORT3D_H
#define VIEWPORT3D_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QVector3D>
#include <QElapsedTimer>

#include "perspectivecamera.h"
#include "transmissioncable.h"

class Viewport3D : public QOpenGLWidget
{
    Q_OBJECT

public:
    Viewport3D(QWidget* parent = nullptr);
    virtual ~Viewport3D() {};


signals:
    void cameraMoved(QVector3D newPos);

private:
    virtual void initializeGL() override;
    virtual void paintGL() override;
    virtual void keyPressEvent(QKeyEvent* event) override;
    virtual void keyReleaseEvent(QKeyEvent* event) override;
    virtual void timerEvent(QTimerEvent* event) override;

private:
    QOpenGLFunctions_3_3_Core* m_GLFuncs;
    PerspectiveCamera          m_Camera;
    QMap<int, bool>            m_Keys;
    QElapsedTimer              m_Timer;

    //TODO separate this and maybe make a scene graph (?)
    TransmissionCable* cable;
};

#endif // VIEWPORT3D_H

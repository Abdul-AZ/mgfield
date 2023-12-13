#ifndef VIEWPORT3D_H
#define VIEWPORT3D_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QVector3D>
#include <QElapsedTimer>
#include <QOpenGLDebugLogger>
#include <QMatrix4x4>

#include "perspectivecamera.h"
#include "orbitcameracontroller.h"
#include "vectorfield3d.h"
#include "src/grid3d.h"

enum class CameraControlMode
{
    Orbit,
    FreeRoam
};

class Viewport3D : public QOpenGLWidget
{
    Q_OBJECT

public:
    Viewport3D(QWidget* parent = nullptr);
    virtual ~Viewport3D() {};

public slots:
    void cameraModeChanged(CameraControlMode newMode);

private slots:
    void messageLogged(const QOpenGLDebugMessage &debugMessage);
    void showContextMenu(QPoint mousePos);
    void saveFrameAsImage();

signals:
    void cameraMoved(QVector3D newPos);
    void exportedImage(QString path);

private:
    virtual void initializeGL() override;
    virtual void paintGL() override;
    virtual void resizeGL(int width, int height) override;
    virtual void keyPressEvent(QKeyEvent* event) override;
    virtual void keyReleaseEvent(QKeyEvent* event) override;
    virtual void timerEvent(QTimerEvent* event) override;
    virtual void mousePressEvent (QMouseEvent* event) override;
    virtual void mouseReleaseEvent (QMouseEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void wheelEvent(QWheelEvent* event) override;

public:
    Scene SceneData;

private:
    QOpenGLFunctions_3_3_Core* m_GLFuncs;
    PerspectiveCamera          m_Camera;
    QMap<int, bool>            m_Keys;
    QElapsedTimer              m_Timer;
    QMatrix4x4                 m_ProjectionMatrix;
    QPoint                     m_MouseLastPosition;
    OrbitCameraController      m_OrbitCameraController;
    CameraControlMode          m_CameraMode = CameraControlMode::Orbit;
    bool                       m_CapturingMouseDelta = false;

    VectorField3D* m_SimulationVectorField;
    Grid3D*        m_Grid;
};

#endif // VIEWPORT3D_H

#ifndef VIEWPORT3D_H
#define VIEWPORT3D_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QVector3D>
#include <QElapsedTimer>
#include <QOpenGLDebugLogger>
#include <QMatrix4x4>

#include "perspectivecamera.h"
#include "../orbitcameracontroller.h"
#include "../freeroamcameracontroller.h"
#include "vectorfield3d.h"
#include "src/grid3d.h"
#include "viewportsettings.h"

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

    void HookViewportSettings(ViewportSettings* settings);

public slots:
    void cameraModeChanged(CameraControlMode newMode);
    void SceneLoaded(Scene* scene);
    void RequestAddObject();
    void RequestRemoveObject(int32_t index);
    void Redraw();

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

private:
    Scene*                     m_CurrentScene = nullptr;
    QOpenGLFunctions_3_3_Core* m_GLFuncs;
    PerspectiveCamera          m_Camera;
    QElapsedTimer              m_Timer;
    QMatrix4x4                 m_ProjectionMatrix;
    QPoint                     m_MouseLastPosition;
    OrbitCameraController      m_OrbitCameraController;
    FreeRoamCameraController   m_FreeRoamCameraController;
    CameraControlMode          m_CameraMode = CameraControlMode::Orbit;
    bool                       m_CapturingMouseDelta = false;
    ViewportSettings*          m_ViewportSettings = nullptr;

    VectorField3D* m_SimulationVectorField;
    Grid3D*        m_Grid;
};

#endif // VIEWPORT3D_H

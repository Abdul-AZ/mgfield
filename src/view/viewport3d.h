#ifndef VIEWPORT3D_H
#define VIEWPORT3D_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QVector3D>
#include <QElapsedTimer>
#include <QOpenGLDebugLogger>
#include <QMatrix4x4>

#include "../perspectivecamera.h"
#include "../orbitcameracontroller.h"
#include "../freeroamcameracontroller.h"
#include "src/vectorfield3d.h"
#include "src/grid3d.h"
#include "src/scene/objectrenderer.h"
#include "viewportsettings.h"

#define VIEWPORT3D_INITIAL_CAMERA_POSITION        (QVector3D{ 4.0f, 1.0f, 1.5f })

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
    void SceneLoaded(Scene* scene);
    void Redraw();

private slots:
    void messageLogged(const QOpenGLDebugMessage &debugMessage);
    void showContextMenu(QPoint mousePos);
    void saveFrameAsImage();

signals:
    void cameraMoved(QVector3D newPos);
    void exportedImage(QString path);
    void ObjectSelected(std::shared_ptr<Object> object);

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
    virtual bool event(QEvent* event) override;

    void DrawGradient();
    std::shared_ptr<Object> GetHoveredObject();

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

    VectorField3D* m_SimulationVectorField;
    Grid3D*        m_Grid;

    std::unique_ptr<ObjectRenderer> m_ObjectRenderer = nullptr;
};

#endif // VIEWPORT3D_H

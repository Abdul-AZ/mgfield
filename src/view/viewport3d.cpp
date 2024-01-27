#include "viewport3d.h"
#include <QApplication>
#include <QMessageBox>
#include <QKeyEvent>
#include <QDebug>
#include <QMenu>
#include <QFileDialog>

Viewport3D::Viewport3D(QWidget* parent) : QOpenGLWidget(parent)
{
    setFocusPolicy(Qt::ClickFocus);

    startTimer(1000/30);
    m_Timer.restart();

    setContextMenuPolicy(Qt::CustomContextMenu);

    connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(QPoint)));

}

void Viewport3D::messageLogged(const QOpenGLDebugMessage &debugMessage)
{
    qCritical() << debugMessage.message();
}

void Viewport3D::initializeGL()
{
    if (context()->format().majorVersion() < 4)
    {
        if ( context()->format().minorVersion() < 3)
        {
            QMessageBox msgBox;
            msgBox.setText("ERROR: OpenGL 3.3+ is not supported by this system.");
            msgBox.setDefaultButton(QMessageBox::Ok);
            msgBox.exec();

            QCoreApplication::quit();
        }
    }

    m_GLFuncs = new QOpenGLFunctions_3_3_Core;
    m_GLFuncs->initializeOpenGLFunctions();

#ifdef QT_DEBUG
    QOpenGLDebugLogger *logger = new QOpenGLDebugLogger(this);

    logger->initialize();
    logger->startLogging();

    connect(logger, SIGNAL(messageLogged(QOpenGLDebugMessage)), this, SLOT(messageLogged(QOpenGLDebugMessage)));
#endif
    m_GLFuncs->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    m_GLFuncs->glEnable(GL_DEPTH_TEST);
    m_GLFuncs->glEnable(GL_BLEND);
    m_GLFuncs->glClearColor(0.1, 0.1, 0.1, 1.0);

    emit cameraMoved(m_Camera.Position);

    m_SimulationVectorField = new VectorField3D(m_GLFuncs);
    m_Grid = new Grid3D(m_GLFuncs);

    connect(m_SimulationVectorField, SIGNAL(repaintRequested()), this, SLOT(repaint()));
}


//TODO separate rendering and data so that adding objects does not happen in this class
//Maybe have a SceneRenderer class which has all the data?
void Viewport3D::RequestAddObject()
{
    makeCurrent();
    m_CurrentScene->Objects.append(std::make_shared<TransmissionCable>(nullptr));

    emit m_CurrentScene->ObjectAdded();
    repaint();
}

void Viewport3D::RequestRemoveObject(int32_t index)
{
    makeCurrent();
    m_CurrentScene->Objects.remove(index);

    emit m_CurrentScene->ObjectRemoved();
    repaint();
}

void Viewport3D::SceneLoaded(Scene* scene)
{
    m_CurrentScene = scene;
}

void Viewport3D::paintGL()
{
    QMatrix4x4 viewProjection = m_ProjectionMatrix * m_Camera.GetViewMatrix();

    if(m_ViewportSettings)
    {
        if(m_ViewportSettings->getGridEnabled())
            m_Grid->Draw(viewProjection);
    }
    else
        m_Grid->Draw(viewProjection);

    if(m_CurrentScene)
    {
        for (auto& object : m_CurrentScene->Objects)
        {
            if(object->Type == CurrentCarryingCable)
                ((TransmissionCable*)(object.get()))->Draw(viewProjection, m_GLFuncs);
        }
    }

    m_SimulationVectorField->Draw(viewProjection);
}

void Viewport3D::resizeGL(int width, int height)
{
    m_ProjectionMatrix.setToIdentity();
    m_ProjectionMatrix.perspective(45.0f, (float)width / height, 0.001f, 1000.0f);
}

void Viewport3D::keyPressEvent(QKeyEvent *event)
{
    if(m_CameraMode == CameraControlMode::FreeRoam)
        m_FreeRoamCameraController.handleKeyPress(event->key());
}

void Viewport3D::keyReleaseEvent(QKeyEvent *event)
{
    if(m_CameraMode == CameraControlMode::FreeRoam)
        m_FreeRoamCameraController.handleKeyRelease(event->key());
}

void Viewport3D::cameraModeChanged(CameraControlMode newMode)
{
    m_CameraMode = newMode;
}

void Viewport3D::timerEvent(QTimerEvent*)
{
    bool positionChanged = false;
    if(m_CameraMode == CameraControlMode::FreeRoam)
    {
        positionChanged = m_FreeRoamCameraController.handleCameraMovement(m_Camera, m_Timer.elapsed() / 1000.0f);
    }

    if(positionChanged)
    {
        repaint();
        emit cameraMoved(m_Camera.Position);
    }

    m_Timer.restart();
}

void Viewport3D::mousePressEvent(QMouseEvent* event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_MouseLastPosition = event->pos();
        m_CapturingMouseDelta = true;
    }
}

void Viewport3D::mouseReleaseEvent(QMouseEvent* event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_CapturingMouseDelta = false;
    }
}

void Viewport3D::mouseMoveEvent(QMouseEvent *event)
{
    if(!m_CapturingMouseDelta)
        return;

    QPoint delta = event->pos() - m_MouseLastPosition;

    if(m_CameraMode == CameraControlMode::Orbit)
    {
        m_OrbitCameraController.handleCameraMouseDrag(m_Camera, {(float)delta.x(), (float)delta.y()});

        repaint();
        emit cameraMoved(m_Camera.Position);
    }

    m_MouseLastPosition = event->pos();
}

void Viewport3D::wheelEvent(QWheelEvent* event)
{
    if(m_CameraMode == CameraControlMode::Orbit)
    {
        m_OrbitCameraController.handleCameraMouseScroll(m_Camera, event->angleDelta().y());

        repaint();
        emit cameraMoved(m_Camera.Position);
    }
}

void Viewport3D::showContextMenu(QPoint mousePos)
{
    QMenu contextMenu(tr("Context menu"), this);

    QAction action1("Save Image", this);
    connect(&action1, SIGNAL(triggered()), this, SLOT(saveFrameAsImage()));
    contextMenu.addAction(&action1);

    contextMenu.exec(mapToGlobal(mousePos));
}

void Viewport3D::saveFrameAsImage()
{
    auto image = grabFramebuffer();

    QString path = QFileDialog::getSaveFileName(this, tr("Save Image File"), QString(), tr("Images (*.png)"));

    if (!path.isEmpty())
    {
        if(!path.endsWith(".png"))
            path.append(".png");

        image.save(path);
        emit exportedImage(path);
    }
    else
        emit exportedImage("Saving cancelled");
}

void Viewport3D::HookViewportSettings(ViewportSettings* settings)
{
    m_ViewportSettings = settings;

    connect(settings, &ViewportSettings::SettingsChanged, this, [this] ()  {repaint(); });
}
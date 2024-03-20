#include "viewport3d.h"
#include <QApplication>
#include <QMessageBox>
#include <QKeyEvent>
#include <QDebug>
#include <QMenu>
#include <QFileDialog>
#include <QPainter>
#include <QPainterPath>
#include <QString>

#include "thirdparty/eng_format/eng_format.hpp"

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
    if(debugMessage.severity() == QOpenGLDebugMessage::HighSeverity)
        qCritical() << debugMessage.message();
    else if(debugMessage.severity() == QOpenGLDebugMessage::MediumSeverity)
        qWarning() << debugMessage.message();
}

void Viewport3D::initializeGL()
{
    if (context()->format().majorVersion() < 4)
    {
        if ( context()->format().minorVersion() < 6)
        {
            QMessageBox msgBox;
            msgBox.setText("ERROR: OpenGL 4.6 is not supported by this system.");
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
    m_GLFuncs->glEnable(GL_STENCIL_TEST);
    m_GLFuncs->glEnable(GL_BLEND);
    m_GLFuncs->glClearColor(0.1, 0.1, 0.1, 1.0);

    m_OrbitCameraController.ClipPositionToOrbit(m_Camera, VIEWPORT3D_INITIAL_CAMERA_POSITION);
    emit cameraMoved(m_Camera.Position);

    m_SimulationVectorField = new VectorField3D(m_GLFuncs);
    m_Grid = new Grid3D(m_GLFuncs);

    connect(m_SimulationVectorField, SIGNAL(repaintRequested()), this, SLOT(repaint()));

    m_ObjectRenderer = std::make_unique<ObjectRenderer>();
}

void Viewport3D::SceneLoaded(Scene* scene)
{
    m_CurrentScene = scene;

    connect(scene, &Scene::ObjectAdded, this, &Viewport3D::Redraw);
    connect(scene, &Scene::ObjectRemoved, this, &Viewport3D::Redraw);
}

void Viewport3D::paintGL()
{
    QMatrix4x4 viewProjection = m_ProjectionMatrix * m_Camera.GetViewMatrix();
    m_GLFuncs->glClearStencil(VIEWPORT3D_STENCIL_BUFFER_NO_OBJECT_VALUE);
    m_GLFuncs->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    m_GLFuncs->glEnable(GL_DEPTH_TEST);
    m_GLFuncs->glEnable(GL_BLEND);
    m_GLFuncs->glEnable(GL_STENCIL_TEST);

    m_GLFuncs->glStencilMask(0x00);
    if(m_ViewportSettings)
    {
        if(m_ViewportSettings->getGridEnabled())
            m_Grid->Draw(viewProjection);
    }
    else
        m_Grid->Draw(viewProjection);

    // Enable stencil buffer
    m_GLFuncs->glStencilMask(0xFF);
    m_GLFuncs->glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    if(m_CurrentScene)
        m_ObjectRenderer->DrawScene(context(), m_CurrentScene, viewProjection);

    m_GLFuncs->glStencilFunc(GL_ALWAYS, VIEWPORT3D_STENCIL_BUFFER_NO_OBJECT_VALUE, VIEWPORT3D_STENCIL_BUFFER_NO_OBJECT_VALUE);

    m_SimulationVectorField->Draw(viewProjection);

    if(m_ViewportSettings)
    {
        if(m_ViewportSettings->getGradientEnabled())
            DrawGradient();
    }
}

void Viewport3D::DrawGradient()
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QPoint topLeft(20, 80);
    QPoint bottomRight(50, height() - 80);

    painter.drawText(topLeft + QPoint(35, fontMetrics().capHeight()), "MAX");
    std::string maxValue = to_engineering_string(SIM_CONSTANT_VACUUM_MAGNETIC_PERMEABILITY * MFSimulator::GetInstance()->SimulationResultsMaxMagnitude, 3, eng_prefixed, "T");
    painter.drawText(topLeft + QPoint(35, fontMetrics().capHeight() * 2 + 3), QString::fromStdString(maxValue));
    painter.drawText(bottomRight + QPoint(5, 0), "MIN");
    std::string minValue = to_engineering_string(SIM_CONSTANT_VACUUM_MAGNETIC_PERMEABILITY * MFSimulator::GetInstance()->SimulationResultsMinMagnitude, 3, eng_prefixed, "T");
    painter.drawText(bottomRight + QPoint(5, -fontMetrics().capHeight() - 3), QString::fromStdString(minValue));

    // Set border size
    QPen pen(Qt::black, 2);
    painter.setPen(pen);

    QLinearGradient m_gradient(topLeft, bottomRight);

    m_gradient.setColorAt(0.0, Qt::red);
    m_gradient.setColorAt(1.0, Qt::blue);
    painter.fillRect(QRect(topLeft, bottomRight), m_gradient);
    painter.drawRect(QRect(topLeft, bottomRight));

    painter.end();
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

        makeCurrent();

        uint8_t value = 0;

        m_GLFuncs->glReadPixels(event->pos().x(), height() - event->pos().y(), 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, &value);

        if(value == VIEWPORT3D_STENCIL_BUFFER_NO_OBJECT_VALUE)
        {
            emit ObjectSelected(nullptr);
        }
        else
        {
            emit ObjectSelected(m_CurrentScene->Objects[value]);
        }
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

void Viewport3D::Redraw()
{
    repaint();
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

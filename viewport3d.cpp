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
    makeCurrent();

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

    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setVersion(3, 3);
    format.setProfile(QSurfaceFormat::CoreProfile);

#ifdef QT_DEBUG
    format.setOption(QSurfaceFormat::DebugContext);
#endif

    QSurfaceFormat::setDefaultFormat(format);
    context()->setFormat(format);
    context()->create();

    makeCurrent();
    m_GLFuncs = new QOpenGLFunctions_3_3_Core;
    m_GLFuncs->initializeOpenGLFunctions();

#ifdef QT_DEBUG
    QOpenGLDebugLogger *logger = new QOpenGLDebugLogger(this);

    logger->initialize();
    logger->startLogging();

    connect(logger, SIGNAL(messageLogged(QOpenGLDebugMessage)), this, SLOT(messageLogged(QOpenGLDebugMessage)));
#endif

    m_GLFuncs->glEnable(GL_DEPTH_TEST);
    m_GLFuncs->glClearColor(0.1, 0.1, 0.1, 1.0);

    emit cameraMoved(m_Camera.Position);

    cable = new TransmissionCable(m_GLFuncs, m_Camera);
    vectorField = new VectorField3D(m_GLFuncs);
}

void Viewport3D::paintGL()
{
    QMatrix4x4 viewProjection = m_ProjectionMatrix * m_Camera.GetViewMatrix();

    cable->Draw(viewProjection);
    vectorField->Draw(viewProjection);
}

void Viewport3D::resizeGL(int width, int height)
{
    m_ProjectionMatrix.setToIdentity();
    m_ProjectionMatrix.perspective(45.0f, (float)width / height, 0.001f, 1000.0f);
}

void Viewport3D::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
        case Qt::Key_W:
        case Qt::Key_A:
        case Qt::Key_S:
        case Qt::Key_D:
        case Qt::Key_Space:
        case Qt::Key_Shift:
            m_Keys[event->key()] = true;
    }
}

void Viewport3D::keyReleaseEvent(QKeyEvent *event)
{
    switch (event->key())
    {
        case Qt::Key_W:
        case Qt::Key_A:
        case Qt::Key_S:
        case Qt::Key_D:
        case Qt::Key_Space:
        case Qt::Key_Shift:
            m_Keys[event->key()] = false;
    }
}

void Viewport3D::cameraModeChanged(CameraControlMode newMode)
{
    m_CameraMode = newMode;
}

void Viewport3D::timerEvent(QTimerEvent*)
{
    const float factor = 3.0f;

    bool positionChanged = false;
    if(m_CameraMode == CameraControlMode::FreeRoam)
    {
        if(m_Keys[Qt::Key_W])
        {
            m_Camera.Position.setZ(m_Camera.Position.z() - factor * m_Timer.elapsed() / 1000.0f);

            positionChanged = true;
        }

        if(m_Keys[Qt::Key_A])
        {
            m_Camera.Position.setX(m_Camera.Position.x() - factor * m_Timer.elapsed() / 1000.0f);

            positionChanged = true;
        }

        if(m_Keys[Qt::Key_S])
        {
            m_Camera.Position.setZ(m_Camera.Position.z() + factor * m_Timer.elapsed() / 1000.0f);

            positionChanged = true;
        }

        if(m_Keys[Qt::Key_D])
        {
            m_Camera.Position.setX(m_Camera.Position.x() + factor * m_Timer.elapsed() / 1000.0f);

            positionChanged = true;
        }

        if(m_Keys[Qt::Key_Space])
        {
            m_Camera.Position.setY(m_Camera.Position.y() + factor * m_Timer.elapsed() / 1000.0f);

            positionChanged = true;
        }

        if(m_Keys[Qt::Key_Shift])
        {
            m_Camera.Position.setY(m_Camera.Position.y() - factor * m_Timer.elapsed() / 1000.0f);

            positionChanged = true;
        }
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

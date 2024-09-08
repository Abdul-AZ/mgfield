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
#include <QSettings>
#include <QToolTip>

#include "thirdparty/eng_format/eng_format.hpp"
#include "src/sim/mfsimulator.h"

#include <src/config.h>
#include <reactphysics3d/reactphysics3d.h>
#include <magic_enum.hpp>

Viewport3D::Viewport3D(QWidget* parent) : QOpenGLWidget(parent)
{
    setFocusPolicy(Qt::ClickFocus);

    startTimer(1000/30);
    m_Timer.restart();

    setContextMenuPolicy(Qt::CustomContextMenu);

    connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(QPoint)));
    MFSimulator* simulator = MFSimulator::GetInstance();
    connect(simulator, &MFSimulator::SimulationFinished, this, &Viewport3D::Redraw);
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
    if (context()->format().majorVersion() < 3)
    {
        if ( context()->format().minorVersion() < 3)
        {
            QMessageBox msgBox;
            msgBox.setText("ERROR: OpenGL 3.3 is not supported by this system.");
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
#if	CONFIG_MULTISAMPLE_ENABLE
    m_GLFuncs->glEnable(GL_MULTISAMPLE);
#endif
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
    QSettings settings;

    QMatrix4x4 viewProjection = m_ProjectionMatrix * m_Camera.GetViewMatrix();
    m_SimulationVectorField->StartFrame(viewProjection);
    m_GLFuncs->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_GLFuncs->glEnable(GL_DEPTH_TEST);
    m_GLFuncs->glEnable(GL_BLEND);

    if(settings.value("ViewportSettings/ShowGrid", true).toBool())
        m_Grid->Draw(viewProjection);

    if(m_CurrentScene)
        m_ObjectRenderer->DrawScene(context(), m_CurrentScene, m_SimulationVectorField, viewProjection);

    m_SimulationVectorField->AddSimulationResultArrows();
    m_SimulationVectorField->EndFrame();

    if(settings.value("ViewportSettings/ShowGradient", true).toBool())
        DrawGradient();
}

void Viewport3D::DrawGradient()
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QPoint topLeft(20, 80);
    QPoint bottomRight(50, height() - 80);

    // Draw background as a rounded rectangle
    QRect background = QRect(topLeft + QPoint(-10,-10), bottomRight + QPoint(50,10));
    QPainterPath path;
    path.addRoundedRect(background, 10, 10);
    QPen pen(Qt::black, 1);
    painter.setPen(pen);
    painter.fillPath(path, QApplication::palette().base().color());
    painter.drawPath(path);

    // Draw text displaying minimum and maximum values for the gradient
    painter.setPen(QApplication::palette().text().color());
    painter.drawText(topLeft + QPoint(35, fontMetrics().capHeight()), "MAX");
    std::string maxValue = to_engineering_string(SIM_CONSTANT_VACUUM_MAGNETIC_PERMEABILITY * MFSimulator::GetInstance()->SimulationResultsMaxMagnitude, 3, eng_prefixed, "T");
    painter.drawText(topLeft + QPoint(35, fontMetrics().capHeight() * 2 + 3), QString::fromStdString(maxValue));
    painter.drawText(bottomRight + QPoint(5, 0), "MIN");
    std::string minValue = to_engineering_string(SIM_CONSTANT_VACUUM_MAGNETIC_PERMEABILITY * MFSimulator::GetInstance()->SimulationResultsMinMagnitude, 3, eng_prefixed, "T");
    painter.drawText(bottomRight + QPoint(5, -fontMetrics().capHeight() - 3), QString::fromStdString(minValue));

    // Draw gradient in a rectangle
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

        auto obj = GetHoveredObject();
        emit ObjectSelected(obj);
    }
}

std::shared_ptr<Object> Viewport3D::GetHoveredObject()
{
    auto pos = QCursor::pos();
    pos = mapFromGlobal(pos);

    auto invVP = (m_ProjectionMatrix * m_Camera.GetViewMatrix()).inverted();
    QVector4D startScreenCoord((pos.x() - width() / 2) * 2.0 / width(),  (height() / 2 -pos.y()) * 2.0 / height(), 0.0f, 1.0f);
    QVector4D endScreenCoord((pos.x() - width() / 2) * 2.0 / width(),  (height() / 2 -pos.y()) * 2.0 / height(), 1.0f, 1.0f);

    QVector4D startWorldPos = invVP * startScreenCoord;
    startWorldPos = startWorldPos / startWorldPos.w();

    QVector4D endWorldPos = invVP * endScreenCoord;
    endWorldPos = endWorldPos / endWorldPos.w();

    reactphysics3d::Vector3 start = {startWorldPos.x(), startWorldPos.y(), startWorldPos.z()};
    reactphysics3d::Vector3 end = {endWorldPos.x(), endWorldPos.y(), endWorldPos.z()};

    reactphysics3d::Ray ray(start, end);

    bool hitObject = false;
    for(auto& obj : m_CurrentScene->Objects)
    {
        if(obj->Raycast(ray))
        {
            return obj;
        }
    }
    return nullptr;
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

bool Viewport3D::event(QEvent* event)
{
    if (event->type() == QEvent::ToolTip) {

        QHelpEvent *helpEvent = static_cast<QHelpEvent *>(event);

        auto obj = GetHoveredObject();

        if(obj)
        {
            QString str;
            QDebug(&str) << "Name: " << obj->Name << "\n";
            QDebug(&str) << magic_enum::enum_name(obj->Type) << "\n";
            QDebug(&str) << "Position: " << obj->Position;

            QToolTip::showText(helpEvent->globalPos(), str);
        }
        else
            QToolTip::showText(helpEvent->globalPos(), "No Object");

        return true;
    }

    return QWidget::event(event);
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

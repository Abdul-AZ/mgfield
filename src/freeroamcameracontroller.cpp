#include "freeroamcameracontroller.h"

FreeRoamCameraController::FreeRoamCameraController() {}


void FreeRoamCameraController::handleKeyPress(int32_t key)
{
    switch (key)
    {
        case Qt::Key_W:
        case Qt::Key_A:
        case Qt::Key_S:
        case Qt::Key_D:
        case Qt::Key_Space:
        case Qt::Key_Shift:
            m_Keys[key] = true;
    }
}

void FreeRoamCameraController::handleKeyRelease(int32_t key)
{
    switch (key)
    {
        case Qt::Key_W:
        case Qt::Key_A:
        case Qt::Key_S:
        case Qt::Key_D:
        case Qt::Key_Space:
        case Qt::Key_Shift:
            m_Keys[key] = false;
    }
}

bool FreeRoamCameraController::handleCameraMovement(PerspectiveCamera& camera, float deltaTime)
{
    bool positionChanged = false;

    if(m_Keys[Qt::Key_W])
    {
        camera.Position += camera.Direction.normalized() * FREEROAM_CAMERA_CONTROLLER_MOVEMENT_SPEED_FACTOR * deltaTime;

        positionChanged = true;
    }

    if(m_Keys[Qt::Key_A])
    {
        camera.Position -= QVector3D::crossProduct(camera.Direction, PERSPECTIVE_CAMERA_GLOBAL_UP).normalized() * FREEROAM_CAMERA_CONTROLLER_MOVEMENT_SPEED_FACTOR * deltaTime;

        positionChanged = true;
    }

    if(m_Keys[Qt::Key_S])
    {
        camera.Position -= camera.Direction.normalized() * FREEROAM_CAMERA_CONTROLLER_MOVEMENT_SPEED_FACTOR * deltaTime;

        positionChanged = true;
    }

    if(m_Keys[Qt::Key_D])
    {
        camera.Position += QVector3D::crossProduct(camera.Direction, PERSPECTIVE_CAMERA_GLOBAL_UP).normalized() * FREEROAM_CAMERA_CONTROLLER_MOVEMENT_SPEED_FACTOR * deltaTime;

        positionChanged = true;
    }

    if(m_Keys[Qt::Key_Space])
    {
        camera.Position.setY(camera.Position.y() + FREEROAM_CAMERA_CONTROLLER_MOVEMENT_SPEED_FACTOR * deltaTime);

        positionChanged = true;
    }

    if(m_Keys[Qt::Key_Shift])
    {
        camera.Position.setY(camera.Position.y() - FREEROAM_CAMERA_CONTROLLER_MOVEMENT_SPEED_FACTOR * deltaTime);

        positionChanged = true;
    }

    return positionChanged;
}

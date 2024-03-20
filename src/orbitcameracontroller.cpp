#include "orbitcameracontroller.h"


void OrbitCameraController::ClipPositionToOrbit(PerspectiveCamera& camera, QVector3D position)
{
    camera.Position = position;
    handleCameraMouseDrag(camera, QVector2D(0.0f, 0.0f));
}

void OrbitCameraController::handleCameraMouseDrag(PerspectiveCamera& camera, QVector2D mouseDelta)
{
    float azimuthAngle = std::atan2(camera.Position.z(), camera.Position.x());
    float polarAngle = std::atan2(sqrt(camera.Position.x() * camera.Position.x() + camera.Position.z() * camera.Position.z()), camera.Position.y());

    azimuthAngle += 0.01f * mouseDelta.x();
    polarAngle -= 0.01f * mouseDelta.y();

    polarAngle = std::clamp(polarAngle, 1.0f, (float)M_PI - 1.0f);
    camera.Position.setX(m_OrbitRadius * sin(polarAngle) * cos(azimuthAngle));
    camera.Position.setZ(m_OrbitRadius * sin(polarAngle) * sin(azimuthAngle));
    camera.Position.setY(m_OrbitRadius * cos(polarAngle));

    camera.Direction = (m_Target - camera.Position).normalized();
}

void OrbitCameraController::handleCameraMouseScroll(PerspectiveCamera& camera, int32_t scroll)
{
    m_OrbitRadius -= scroll * ORBIT_CAMERA_CONTROLLER_ZOOM_SPEED;
    m_OrbitRadius = std::clamp(m_OrbitRadius, ORBIT_CAMERA_CONTROLLER_ZOOM_MIN, ORBIT_CAMERA_CONTROLLER_ZOOM_MAX);

    float azimuthAngle = std::atan2(camera.Position.z(), camera.Position.x());
    float polarAngle = std::atan2(sqrt(camera.Position.x() * camera.Position.x() + camera.Position.z() * camera.Position.z()), camera.Position.y());

    polarAngle = std::clamp(polarAngle, 1.0f, (float)M_PI - 1.0f);
    camera.Position.setX(m_OrbitRadius * sin(polarAngle) * cos(azimuthAngle));
    camera.Position.setZ(m_OrbitRadius * sin(polarAngle) * sin(azimuthAngle));
    camera.Position.setY(m_OrbitRadius * cos(polarAngle));
}

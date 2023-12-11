#ifndef ORBITCAMERACONTROLLER_H
#define ORBITCAMERACONTROLLER_H

#include "perspectivecamera.h"

#define ORBIT_CAMERA_CONTROLLER_DEFAULT_RADIUS (5.0f)
#define ORBIT_CAMERA_CONTROLLER_ZOOM_SPEED     (0.01f)
#define ORBIT_CAMERA_CONTROLLER_ZOOM_MIN       (5.0f )
#define ORBIT_CAMERA_CONTROLLER_ZOOM_MAX       (50.0f)

class OrbitCameraController
{
public:
    void handleCameraMouseDrag(PerspectiveCamera& camera, QVector2D mouseDelta);
    void handleCameraMouseScroll(PerspectiveCamera& camera, int32_t scroll);

private:
    float m_OrbitRadius = ORBIT_CAMERA_CONTROLLER_DEFAULT_RADIUS;
};

#endif // ORBITCAMERACONTROLLER_H

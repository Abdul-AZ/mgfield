#ifndef FREEROAMCAMERACONTROLLER_H
#define FREEROAMCAMERACONTROLLER_H

#include "perspectivecamera.h"

#define FREEROAM_CAMERA_CONTROLLER_MOVEMENT_SPEED_FACTOR (3.0f)

class FreeRoamCameraController
{
public:
    FreeRoamCameraController();

    void handleKeyPress(int32_t key);
    void handleKeyRelease(int32_t key);
    bool handleCameraMovement(PerspectiveCamera& camera, float deltaTime);

private:
    QMap<int, bool> m_Keys;
};

#endif // FREEROAMCAMERACONTROLLER_H

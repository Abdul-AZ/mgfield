#ifndef PERSPECTIVECAMERA_H
#define PERSPECTIVECAMERA_H

#include <QtMath>
#include <QVector3D>
#include <QMatrix4x4>

#define PERSPECTIVE_CAMERA_GLOBAL_UP (QVector3D(0.0f, 1.0f, 0.0f))

class PerspectiveCamera
{
public:
    PerspectiveCamera();

    QMatrix4x4 GetViewMatrix();

    QVector3D Position  = QVector3D(0, 0, 5.0f);
    QVector3D Direction = QVector3D(0, 0, 1);
    QVector3D Up        = QVector3D(0, 1, 0);
};

#endif // PERSPECTIVECAMERA_H

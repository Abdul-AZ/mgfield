#ifndef PERSPECTIVECAMERA_H
#define PERSPECTIVECAMERA_H

#include <QtMath>
#include <QVector3D>
#include <QMatrix4x4>

class PerspectiveCamera
{
public:
    PerspectiveCamera();

    QMatrix4x4 GetViewMatrix();

    QVector3D Position = QVector3D(0, 0, 20.0f);
    QVector3D Forward  = QVector3D(0, 0, -1);
    QVector3D Up       = QVector3D(0, 1, 0);
};

#endif // PERSPECTIVECAMERA_H

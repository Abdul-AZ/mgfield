#ifndef PERSPECTIVECAMERA_H
#define PERSPECTIVECAMERA_H

#include <QVector3D>


class PerspectiveCamera
{
public:
    PerspectiveCamera();


    QVector3D Position = QVector3D(0, 0, 0);
};

#endif // PERSPECTIVECAMERA_H

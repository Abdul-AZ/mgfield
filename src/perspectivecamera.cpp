#include "perspectivecamera.h"

PerspectiveCamera::PerspectiveCamera()
{

}


QMatrix4x4 PerspectiveCamera::GetViewMatrix()
{
    QMatrix4x4 mat;

    mat.lookAt(Position, Position + Direction, Up);

    return mat;
}

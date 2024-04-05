#include "straightwireobject.h"

#include <QDebug>

StraightWireObject::StraightWireObject()
    :
    Object(ObjectType::StraightWire)
{
    Name = "Current Carrying Conductor";
}

QVector3D StraightWireObject::GetCurrentFlowVector() const
{
    return Rotation.rotatedVector(QVector3D(1.0f,0.0f,0.0f));
}

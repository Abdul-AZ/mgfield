#include "straightwireobject.h"

#include <QDebug>

StraightWireObject::StraightWireObject()
    :
    Object(ObjectType::StraightWire)
{
    Name = "Current Carrying Conductor";
}

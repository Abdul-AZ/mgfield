#include "currentcarryingsheet.h"

CurrentCarryingSheet::CurrentCarryingSheet() :
    Object(ObjectType::CurrentCarryingSheet)
{
    Name = "Sheet";
}

QVector3D CurrentCarryingSheet::GetNormalVector() const
{
    return Rotation.rotatedVector(QVector3D(0.0f,1.0f,0.0f));
}

QVector3D CurrentCarryingSheet::GetCurrentFlowVector() const
{
    return Rotation.rotatedVector(QVector3D(1.0f,0.0f,0.0f));
}

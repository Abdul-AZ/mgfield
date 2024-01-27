#ifndef OBJECT_H
#define OBJECT_H

#include <QVector3D>

enum ObjectType
{
    CurrentCarryingCable
};

class Object
{
public:
    Object(ObjectType type) : Type(type) {};
    const ObjectType Type;

    QString   Name;
    QVector3D Position;
    QVector3D RotationEular;
    QVector3D Scale;
};

#endif // OBJECT_H

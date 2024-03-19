#ifndef OBJECT_H
#define OBJECT_H

#include <QVector3D>
#include <QQuaternion>

enum class ObjectType
{
    StraightWire,
    CurrentCarryingSheet,
    PermanentMagnet
};

class Object
{
public:
    Object(ObjectType type) : Type(type) {};
    const ObjectType Type;

    QString     Name;
    QVector3D   Position;
    QQuaternion Rotation;
    QVector3D   Scale;
};

#endif // OBJECT_H

#ifndef OBJECT_H
#define OBJECT_H

#include <QVector3D>
#include <QQuaternion>
#include <reactphysics3d/reactphysics3d.h>

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

    virtual bool Raycast(const reactphysics3d::Ray& ray) const { return false; };
    virtual void UpdateColliders() { };

    QString     Name;
    QVector3D   Position;
    QQuaternion Rotation;

    /*!
     *  Stores rotation as euler angles in degrees.
     *  Used by ObjectInspector to avoid calling QQuaternion::toEulerAngles which causes problems
     *  Uses default Qt ordering (pitch, yaw, roll), rotation order: ZXY
     */
    QVector3D   EulerRotation;

    QVector3D   Scale;
};

#endif // OBJECT_H

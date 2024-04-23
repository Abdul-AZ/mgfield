#include "straightwireobject.h"

#include <QDebug>

StraightWireObject::StraightWireObject(reactphysics3d::PhysicsCommon* physicsCommon, reactphysics3d::PhysicsWorld* world)
    :
    Object(ObjectType::StraightWire),
    m_PhysicsCommon(physicsCommon),
    m_PhysicsWorld(world)
{
    Name = "Current Carrying Conductor";

    m_Shape = m_PhysicsCommon->createCapsuleShape(0.1, 100.0f);

    m_Body = world->createRigidBody(reactphysics3d::Transform(reactphysics3d::Vector3(0.0f, 0.0f, 0.0f), reactphysics3d::Quaternion::identity()));
    m_Body->setType(reactphysics3d::BodyType::STATIC);
    m_Body->addCollider(m_Shape, reactphysics3d::Transform(reactphysics3d::Vector3(0.0f, 0.0f, 0.0f), reactphysics3d::Quaternion::identity()));

    UpdateColliders();
}

bool StraightWireObject::Raycast(const reactphysics3d::Ray& ray) const
{
    reactphysics3d::RaycastInfo info;
    m_Body->raycast(ray, info);

    return info.body == m_Body;
}

void StraightWireObject::UpdateColliders()
{
    reactphysics3d::Vector3 position(Position.x(), Position.y(), Position.z());
    QQuaternion actualRot = Rotation * QQuaternion::fromAxisAndAngle({0.0f, 0.0f, 1.0f}, 90.0f);
    reactphysics3d::Quaternion orientation(actualRot.x(), actualRot.y(), actualRot.z(), actualRot.scalar());
    reactphysics3d::Transform transform(position, orientation);

    m_Body->setTransform(transform);
}

QVector3D StraightWireObject::GetCurrentFlowVector() const
{
    return Rotation.rotatedVector(QVector3D(1.0f,0.0f,0.0f));
}

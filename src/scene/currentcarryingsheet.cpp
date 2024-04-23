#include "currentcarryingsheet.h"

CurrentCarryingSheet::CurrentCarryingSheet(reactphysics3d::PhysicsCommon* physicsCommon, reactphysics3d::PhysicsWorld* world) :
    Object(ObjectType::CurrentCarryingSheet),
    m_PhysicsCommon(physicsCommon),
    m_PhysicsWorld(world)
{
    Name = "Sheet";

    m_Shape = m_PhysicsCommon->createBoxShape({ m_Length, 0.01f, m_Width });
    reactphysics3d::Vector3 position(0.0, 0.0, 0.0);
    reactphysics3d::Quaternion orientation = reactphysics3d::Quaternion::identity();
    reactphysics3d::Transform transform(position, orientation);

    m_Body = world->createRigidBody(transform);
    m_Body->setType(reactphysics3d::BodyType::STATIC);
    m_Body->addCollider(m_Shape, transform);
}

bool CurrentCarryingSheet::Raycast(const reactphysics3d::Ray& ray) const
{
    reactphysics3d::RaycastInfo info;
    m_Body->raycast(ray, info);

    return info.body == m_Body;
}

void CurrentCarryingSheet::UpdateColliders()
{
    reactphysics3d::Vector3 position(Position.x(), Position.y(), Position.z());
    reactphysics3d::Quaternion orientation(Rotation.x(), Rotation.y(), Rotation.z(), Rotation.scalar());
    reactphysics3d::Transform transform(position, orientation);

    m_Body->setTransform(transform);
    m_Shape->setHalfExtents({ m_Length, 0.01f, m_Width });
}

CurrentCarryingSheet::~CurrentCarryingSheet()
{
    m_PhysicsCommon->destroyBoxShape(m_Shape);
}

QVector3D CurrentCarryingSheet::GetNormalVector() const
{
    return Rotation.rotatedVector(QVector3D(0.0f,1.0f,0.0f));
}

QVector3D CurrentCarryingSheet::GetCurrentFlowVector() const
{
    return Rotation.rotatedVector(QVector3D(1.0f,0.0f,0.0f));
}

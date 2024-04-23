#ifndef CURRENTCARRYINGSHEET_H
#define CURRENTCARRYINGSHEET_H

#include "object.h"

#include <reactphysics3d/reactphysics3d.h>

class CurrentCarryingSheet : public Object
{
public:
    CurrentCarryingSheet(reactphysics3d::PhysicsCommon* physicsCommon, reactphysics3d::PhysicsWorld* world);
    virtual ~CurrentCarryingSheet();

    virtual bool Raycast(const reactphysics3d::Ray& ray) const override;
    virtual void UpdateColliders() override;

    QVector3D GetNormalVector() const;
    QVector3D GetCurrentFlowVector() const;

    float GetWidth() const { return m_Width; }
    void SetWidth(float width) { m_Width = width; }

    float GetLength() const { return m_Length; }
    void SetLength(float height) { m_Length = height; }

    float GetBr() const { return m_Br; }
    void SetBr(float br) { m_Br = br; }
private:
    float m_Width = 1.0f;
    float m_Length = 1.0f;
    float m_Br = 1.0f;

    reactphysics3d::PhysicsCommon* m_PhysicsCommon;
    reactphysics3d::PhysicsWorld* m_PhysicsWorld;
    reactphysics3d::BoxShape* m_Shape;
    reactphysics3d::RigidBody* m_Body;
};

#endif // CURRENTCARRYINGSHEET_H

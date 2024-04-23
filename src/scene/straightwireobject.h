#ifndef STRAIGHTWIREOBJECT_H
#define STRAIGHTWIREOBJECT_H

#include <QMatrix4x4>
#include "src/scene/object.h"
#include <reactphysics3d/reactphysics3d.h>

#define STRAIGHT_WIRE_OBJECT_MODEL_BASE_SCALE (0.1f)
#define STRAIGHT_WIRE_OBJECT_BASE_ROTATION    QVector3D(-1.0f,0.0f,0.0f)

class StraightWireObject : public Object
{
public:
    StraightWireObject(reactphysics3d::PhysicsCommon* physicsCommon, reactphysics3d::PhysicsWorld* world);
    ~StraightWireObject() {};

    virtual bool Raycast(const reactphysics3d::Ray& ray) const override;
    virtual void UpdateColliders() override;

    QVector3D GetCurrentFlowVector() const;

    float GetDCCurrent() const { return m_DCCurrent; };
    void  SetDCCurrent(float value) { m_DCCurrent = value; };

    bool  GetIsInfiniteLength() const { return m_IsInfiniteLength; };
    void  SetIsInfiniteLength(bool value) { m_IsInfiniteLength = value; };

private:
    void loadModel();

    float                      m_DCCurrent = 1.0f;
    bool                       m_IsInfiniteLength = true;

    reactphysics3d::PhysicsCommon* m_PhysicsCommon;
    reactphysics3d::PhysicsWorld* m_PhysicsWorld;
    reactphysics3d::CapsuleShape* m_Shape;
    reactphysics3d::RigidBody* m_Body;
};

#endif // STRAIGHTWIREOBJECT_H

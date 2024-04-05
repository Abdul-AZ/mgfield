#ifndef STRAIGHTWIREOBJECT_H
#define STRAIGHTWIREOBJECT_H

#include <QMatrix4x4>
#include "src/scene/object.h"

#define STRAIGHT_WIRE_OBJECT_MODEL_BASE_SCALE (0.1f)
#define STRAIGHT_WIRE_OBJECT_BASE_ROTATION    QVector3D(1.0f,0.0f,0.0f)

class StraightWireObject : public Object
{
public:
    StraightWireObject();
    ~StraightWireObject() {};

    QVector3D GetCurrentFlowVector() const;

    float GetDCCurrent() const { return m_DCCurrent; };
    void  SetDCCurrent(float value) { m_DCCurrent = value; };

    bool  GetIsInfiniteLength() const { return m_IsInfiniteLength; };
    void  SetIsInfiniteLength(bool value) { m_IsInfiniteLength = value; };

private:
    void loadModel();

    float                      m_DCCurrent = 1.0f;
    bool                       m_IsInfiniteLength = true;
};

#endif // STRAIGHTWIREOBJECT_H

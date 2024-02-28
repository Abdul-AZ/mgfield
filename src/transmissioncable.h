#ifndef TRANSMISSIONCABLE_H
#define TRANSMISSIONCABLE_H

#include <QMatrix4x4>
#include "src/scene/object.h"

#define TRANSMISSION_CABLE_MODEL_BASE_SCALE (0.1f)

class TransmissionCable : public Object
{
public:
    TransmissionCable();
    ~TransmissionCable() {};

    float GetDCCurrent() const { return m_DCCurrent; };
    void  SetDCCurrent(float value) { m_DCCurrent = value; };

    bool  GetIsInfiniteLength() const { return m_IsInfiniteLength; };
    void  SetIsInfiniteLength(bool value) { m_IsInfiniteLength = value; };

private:
    void loadModel();

    float                      m_DCCurrent = 1.0f;
    bool                       m_IsInfiniteLength = true;
};

#endif // TRANSMISSIONCABLE_H

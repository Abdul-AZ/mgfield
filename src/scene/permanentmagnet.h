#ifndef PERMANENTMAGNET_H
#define PERMANENTMAGNET_H

#include "object.h"

enum class PermanentMagnetShape
{
    BarMagnet
};

class PermanentMagnet : public Object
{
public:
    PermanentMagnet();

    void SetShape(PermanentMagnetShape shape) { this->m_Shape = shape; }
    PermanentMagnetShape GetShape() const { return m_Shape; }

private:
    PermanentMagnetShape m_Shape = PermanentMagnetShape::BarMagnet;
};

#endif // PERMANENTMAGNET_H

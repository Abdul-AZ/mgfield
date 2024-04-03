#ifndef CURRENTCARRYINGSHEET_H
#define CURRENTCARRYINGSHEET_H

#include "object.h"

class CurrentCarryingSheet : public Object
{
public:
    CurrentCarryingSheet();

    QVector3D GetNormalVector() const;

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
};

#endif // CURRENTCARRYINGSHEET_H

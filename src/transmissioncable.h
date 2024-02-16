#ifndef TRANSMISSIONCABLE_H
#define TRANSMISSIONCABLE_H

#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QMatrix4x4>
#include "src/scene/object.h"

#define TRANSMISSION_CABLE_MODEL_BASE_SCALE (0.1f)

class TransmissionCable : public Object
{
public:
    TransmissionCable(QOpenGLFunctions_3_3_Core* funcs);
    ~TransmissionCable() {};

    void Draw(QMatrix4x4 viewProjection, QOpenGLFunctions_3_3_Core* funcs);

    float GetDCCurrent() const { return m_DCCurrent; };
    void  SetDCCurrent(float value) { m_DCCurrent = value; };

    bool  GetIsInfiniteLength() const { return m_IsInfiniteLength; };
    void  SetIsInfiniteLength(bool value) { m_IsInfiniteLength = value; };

private:
    void loadModel();

    float                      m_DCCurrent = 1.0f;
    bool                       m_IsInfiniteLength = true;
    QOpenGLShaderProgram       m_Shader;
    QOpenGLBuffer              m_VertexBuffer;
    QOpenGLBuffer              m_IndexBuffer;
    QOpenGLVertexArrayObject   m_VertexArray;
    int32_t                    m_NumIndecies;
};

#endif // TRANSMISSIONCABLE_H

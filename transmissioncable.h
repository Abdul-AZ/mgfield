#ifndef TRANSMISSIONCABLE_H
#define TRANSMISSIONCABLE_H

#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QMatrix4x4>
#include "perspectivecamera.h"

class TransmissionCable
{
public:
    TransmissionCable(QOpenGLFunctions_3_3_Core* funcs, PerspectiveCamera& camera);

    void Draw(QMatrix4x4 viewProjection);

private:
    void createBuffers();

    QOpenGLFunctions_3_3_Core* m_GLFuncs;
    PerspectiveCamera&         m_Camera;
    QOpenGLShaderProgram       m_Shader;
    QOpenGLBuffer              m_VertexBuffer;
    QOpenGLBuffer              m_IndexBuffer;
    QOpenGLVertexArrayObject   m_VertexArray;
    int32_t                    m_NumIndecies;
};

#endif // TRANSMISSIONCABLE_H

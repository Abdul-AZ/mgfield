#ifndef TRANSMISSIONCABLE_H
#define TRANSMISSIONCABLE_H

#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include "perspectivecamera.h"

class TransmissionCable
{
public:
    TransmissionCable(QOpenGLFunctions_3_3_Core* funcs, PerspectiveCamera& camera);

    void Draw();

    QOpenGLFunctions_3_3_Core* m_GLFuncs;
    PerspectiveCamera&         m_Camera;
    QOpenGLShaderProgram       m_Shader;
    QOpenGLBuffer              m_VertexBuffer;
    QOpenGLVertexArrayObject   m_VertexArray;
};

#endif // TRANSMISSIONCABLE_H

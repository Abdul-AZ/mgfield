#ifndef TRANSMISSIONCABLE_H
#define TRANSMISSIONCABLE_H

#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QMatrix4x4>
#include "perspectivecamera.h"

#define TRANSMISSION_CABLE_BASE_DIRECTION {1.0f, 0.0f, 0.0f}

class TransmissionCable
{
public:
    TransmissionCable(QOpenGLFunctions_3_3_Core* funcs, PerspectiveCamera* camera);

    void Draw(QMatrix4x4 viewProjection);

    QVector3D GetDirection() const { return m_Direction; };
    QVector3D GetPosition() const { return m_Position; };

private:
    void createBuffers();

    QOpenGLFunctions_3_3_Core* m_GLFuncs;
    PerspectiveCamera&         m_Camera;
    QOpenGLShaderProgram       m_Shader;
    QOpenGLBuffer              m_VertexBuffer;
    QOpenGLBuffer              m_IndexBuffer;
    QOpenGLVertexArrayObject   m_VertexArray;
    int32_t                    m_NumIndecies;

    QVector3D                  m_Position  = {0.0f, 0.0f, 0.0f};
    QVector3D                  m_Direction = {0.0f, 1.0f, 1.0f};
};

#endif // TRANSMISSIONCABLE_H

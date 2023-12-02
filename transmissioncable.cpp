#include "transmissioncable.h"

#include <QDebug>

const float VERTICES[] =
{
    -1.0f, -1.0f, 0.0f,
     0.0f,  1.0f, 0.0f,
     1.0f, -1.0f, 0.0f
};

TransmissionCable::TransmissionCable(QOpenGLFunctions_3_3_Core* funcs, PerspectiveCamera& camera)
    :
    m_GLFuncs(funcs),
    m_Camera(camera),
    m_VertexBuffer(QOpenGLBuffer::VertexBuffer)
{

    if(!m_Shader.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/TransmissionCable.vert"))
        qCritical() << "TransmissionLine vertex shader error";

    if(!m_Shader.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/TransmissionCable.frag"))
        qCritical() << "TransmissionLine fragment shader error";

    if(!m_Shader.link())
        qCritical() << "TransmissionLine shader program linking error";

    if(!m_VertexBuffer.create())
        qCritical() << "TransmissionLine vertex buffer could not be created";

    m_VertexArray.create();
    m_VertexArray.bind();

    m_VertexBuffer.bind();
    m_VertexBuffer.allocate(VERTICES, sizeof(VERTICES));

    m_Shader.setAttributeBuffer(0, GL_FLOAT, 0, 3, 0);
    m_Shader.enableAttributeArray(0);

    m_VertexArray.release();
}

void TransmissionCable::Draw()
{
    m_Shader.bind();
    m_VertexArray.bind();

    m_GLFuncs->glDrawArrays(GL_TRIANGLES, 0, 3);

    m_Shader.release();
    m_VertexArray.release();
}

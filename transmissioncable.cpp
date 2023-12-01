#include "transmissioncable.h"

#include <QDebug>

TransmissionCable::TransmissionCable(QOpenGLFunctions_3_3_Core* funcs, PerspectiveCamera& camera)
    : m_GLFuncs(funcs), m_Camera(camera)
{

    if(!m_Shader.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/TransmissionCable.vert"))
        qCritical() << "TransmissionLine vertex shader error";

    if(!m_Shader.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/TransmissionCable.frag"))
        qCritical() << "TransmissionLine fragment shader error";

    if(!m_Shader.link())
        qCritical() << "TransmissionLine shader program linking error";
}



void TransmissionCable::Draw()
{

}

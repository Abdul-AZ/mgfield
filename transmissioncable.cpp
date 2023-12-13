#include "transmissioncable.h"

#include <QDebug>

#define CYLINDER_NUM_VERTICES (6)
#define CYLINDER_HEIGHT       (50.0f)
#define CYLINDER_WIDTH        (0.7f)

TransmissionCable::TransmissionCable(QOpenGLFunctions_3_3_Core* funcs)
    :
    m_GLFuncs(funcs),
    m_VertexBuffer(QOpenGLBuffer::VertexBuffer),
    m_IndexBuffer(QOpenGLBuffer::IndexBuffer)
{

    if(!m_Shader.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/ColoredMesh.vert"))
        qCritical() << "TransmissionLine vertex shader error";

    if(!m_Shader.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/ColoredMesh.frag"))
        qCritical() << "TransmissionLine fragment shader error";

    if(!m_Shader.link())
        qCritical() << "TransmissionLine shader program linking error";

    m_VertexArray.create();
    m_VertexArray.bind();

    createBuffers();

    m_Shader.bind();
    m_Shader.setAttributeBuffer(0, GL_FLOAT, 0, 3, 0);
    m_Shader.enableAttributeArray(0);
    m_Shader.setUniformValue("uMeshColor", QVector4D(1.0f, 1.0f, 1.0f, 1.0f));

    m_VertexArray.release();
}

void TransmissionCable::Draw(QMatrix4x4 viewProjection)
{
    m_Shader.bind();
    m_VertexArray.bind();

    QMatrix4x4 matrix;
    matrix *= viewProjection;


    // Base objection rotation quat
    QQuaternion quat1 = QQuaternion::fromDirection(TRANSMISSION_CABLE_BASE_DIRECTION, {1.0f, 0.0f, 0.0f});
    // Actual object rotation quat
    QQuaternion quat2 = QQuaternion::fromDirection(m_Direction, {1.0f, 0.0f, 0.0f});
    matrix.translate(m_Position);
    matrix.rotate(quat2 * quat1);
    matrix.scale(0.1f);

    m_Shader.setUniformValue(m_Shader.uniformLocation("uModelViewProjection"), matrix);

    m_GLFuncs->glDrawElements(GL_TRIANGLES, m_NumIndecies, GL_UNSIGNED_INT, 0);

    m_Shader.release();
    m_VertexArray.release();
}

void TransmissionCable::createBuffers()
{
    QVector<QVector3D> vertices;
    const float step = 2.0f * M_PI / CYLINDER_NUM_VERTICES;
    float angle = 0.0f;

    vertices.emplace_back(-CYLINDER_HEIGHT, 0.0f, 0.0f);
    for (int i = 0; i < CYLINDER_NUM_VERTICES; i++)
    {
        vertices.emplace_back(-CYLINDER_HEIGHT, CYLINDER_WIDTH * (float)cos(angle), CYLINDER_WIDTH * (float)sin(angle));

        angle += step;
    }

    vertices.emplace_back(CYLINDER_HEIGHT, 0.0f, 0.0f);
    for (int i = 0; i < CYLINDER_NUM_VERTICES; i++)
    {
        vertices.emplace_back(CYLINDER_HEIGHT, CYLINDER_WIDTH * (float)cos(angle), CYLINDER_WIDTH * (float)sin(angle));

        angle += step;
    }

    m_VertexBuffer.create();
    m_VertexBuffer.bind();
    m_VertexBuffer.allocate(vertices.data(), vertices.size() * sizeof(QVector3D));

    QVector<uint32_t> indecies;

    for (int i = 0; i < CYLINDER_NUM_VERTICES; i++)
    {
        indecies.emplace_back(0);
        indecies.emplace_back(i + 1);
        if(i != CYLINDER_NUM_VERTICES - 1)
            indecies.emplace_back(i + 2);
        else
            indecies.emplaceBack(1);
    }

    int32_t offset = CYLINDER_NUM_VERTICES + 1;
    for (int i = 0; i < CYLINDER_NUM_VERTICES; i++)
    {
        indecies.emplace_back(offset);
        indecies.emplace_back(offset + i + 1);
        if(i != CYLINDER_NUM_VERTICES - 1)
            indecies.emplace_back(offset + i + 2);
        else
            indecies.emplaceBack(offset + 1);
    }

    for (int i = 0; i < CYLINDER_NUM_VERTICES; ++i)
    {
        indecies.emplaceBack(i + 1);
        indecies.emplaceBack(i + 2);
        indecies.emplaceBack(offset + i + 1);

        indecies.emplaceBack(i + 2);
        indecies.emplaceBack(offset + i + 1);
        indecies.emplaceBack(offset + i + 2);
    }


    m_IndexBuffer.create();
    m_IndexBuffer.bind();
    m_IndexBuffer.allocate(indecies.data(), indecies.size() * sizeof(uint32_t));
    m_NumIndecies = indecies.size();
}

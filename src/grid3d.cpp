#include "grid3d.h"

Grid3D::Grid3D(QOpenGLFunctions_3_3_Core* funcs) :
    m_GLFuncs(funcs)
{
    if(!m_Shader.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/ColoredMesh.vert"))
        throw std::runtime_error("Error in loading grid vertex shader");

    if(!m_Shader.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/ColoredMesh.frag"))
        throw std::runtime_error("Error in loading grid fragment shader");

    if(!m_Shader.link())
        throw std::runtime_error("Error in linking grid shader program");

    m_VertexArray.create();
    m_VertexArray.bind();

    createBuffers();

    m_Shader.bind();
    m_Shader.setAttributeBuffer(0, GL_FLOAT, 0, 3, 0);
    m_Shader.enableAttributeArray(0);
    m_Shader.setUniformValue("uMeshColor", QVector4D(1.0f, 1.0f, 1.0f, 0.3f));

    m_VertexArray.release();
}

void Grid3D::createBuffers()
{
    QVector3D* vertices = new QVector3D[GRID3D_NUMBER_VERTICES];
    float modelOffset = (GRID3D_NUMBER_LINES - 1) / 2.0f;

    for (int i = 0; i < GRID3D_NUMBER_LINES; i++)
    {
        vertices[i * 2] = QVector3D((float)i - modelOffset, 0.0f,  -modelOffset);
        vertices[i * 2 + 1] = QVector3D((float)i - modelOffset, 0.0f,  modelOffset);
    }

    const int indexOffset = GRID3D_NUMBER_LINES * 2;

    for (int i = 0; i < GRID3D_NUMBER_LINES; i++)
    {
        vertices[i * 2 + indexOffset] = QVector3D(-modelOffset, 0.0f, (float)i - modelOffset);
        vertices[i * 2 + 1 + indexOffset] = QVector3D(modelOffset, 0.0f, (float)i - modelOffset);
    }

    m_VertexBuffer.create();
    m_VertexBuffer.bind();
    m_VertexBuffer.allocate(vertices, GRID3D_NUMBER_VERTICES * sizeof(QVector3D));

    delete[] vertices;
}

void Grid3D::Draw(const QMatrix4x4& viewProjection)
{
    m_Shader.bind();
    m_VertexArray.bind();

    m_Shader.setUniformValue(m_Shader.uniformLocation("uModelViewProjection"), viewProjection);

    m_GLFuncs->glDrawArrays(GL_LINES, 0, GRID3D_NUMBER_VERTICES);

    m_Shader.release();
    m_VertexArray.release();
}

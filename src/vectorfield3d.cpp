#include "vectorfield3d.h"

#include <QFile>
#include "src/modelloader.h"

VectorField3D::VectorField3D(QOpenGLFunctions_3_3_Core* funcs)
    : m_GLFuncs(funcs),
    m_VertexBuffer(QOpenGLBuffer::VertexBuffer),
    m_IndexBuffer(QOpenGLBuffer::IndexBuffer)
{
    GLint maxAllowedSize;
    glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &maxAllowedSize);
    m_MaxNumArrowsPerDrawcall = std::min(maxAllowedSize, (int32_t)VECTOR_FIELD_3D_UBO_MAX_WANTED_SIZE) / VECTOR_FIELD_3D_UBO_ELEMENT_SIZE;

    QFile vshaderSrcFile(":/shaders/VectorField3D.vert");
    vshaderSrcFile.open(QIODeviceBase::ReadOnly);
    std::string vsrc = vshaderSrcFile.readAll().toStdString();
    vshaderSrcFile.close();

    std::string version_line = "#version 330 core";
    auto pos = vsrc.find_first_of(version_line);
    vsrc.insert(version_line.size() + pos, QString("\n#define MAX_ARROW_COUNT (%1)\n").arg(m_MaxNumArrowsPerDrawcall).toStdString());

    m_Shader.create();
    if(!m_Shader.addShaderFromSourceCode(QOpenGLShader::Vertex, vsrc.c_str()))
        qCritical() << "VectorField3D vertex shader error";

    if(!m_Shader.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/VectorField3D.frag"))
        qCritical() << "VectorField3D fragment shader error";

    if(!m_Shader.link())
        qCritical() << "VectorField3D shader program linking error";

    m_VertexArray.create();
    m_VertexArray.bind();

    loadModel();
    m_GLFuncs->glGenBuffers(1, &m_UniformBuffer);
    m_GLFuncs->glBindBuffer(GL_UNIFORM_BUFFER, m_UniformBuffer);

    m_Shader.bind();
    m_Shader.setAttributeBuffer(0, GL_FLOAT, 0, 3, 0);
    m_Shader.enableAttributeArray(0);

    m_Simulator = MFSimulator::GetInstance();
    connect(m_Simulator, SIGNAL(SimulationFinished()), this, SLOT(updateBuffers()));
}

void VectorField3D::loadModel()
{
    ModelLoader::SingleMeshData mesh = ModelLoader::LoadSingleMeshFile(VECTOR_FIELD_3D_ARROW_MODEL_PATH);
    m_VertexBuffer.create();
    m_VertexBuffer.bind();
    m_VertexBuffer.allocate(mesh.Vertices.data(), mesh.Vertices.size() * sizeof(QVector3D));

    m_IndexBuffer.create();
    m_IndexBuffer.bind();
    m_IndexBuffer.allocate(mesh.Indecies.data(), mesh.Indecies.size() * sizeof(uint16_t));
    m_NumIndecies = m_IndexBuffer.size() / sizeof(uint16_t);
}

void VectorField3D::Draw(QMatrix4x4 viewProjection)
{
    m_Shader.bind();
    m_VertexArray.bind();

    m_Shader.setUniformValue(m_Shader.uniformLocation("uViewProjection"), viewProjection);
    uint32_t index = m_GLFuncs->glGetUniformBlockIndex(m_Shader.programId(), "uVectorFieldData");
    m_GLFuncs->glUniformBlockBinding(m_Shader.programId(), index, 2);
    m_GLFuncs->glBindBufferBase(GL_UNIFORM_BUFFER, 2, m_UniformBuffer);

    m_GLFuncs->glDrawElementsInstanced(GL_TRIANGLES, m_NumIndecies, GL_UNSIGNED_SHORT, 0, m_Simulator->SimulationResults.size());

    m_Shader.release();
    m_VertexArray.release();
}

void VectorField3D::updateBuffers()
{
    if(!m_Simulator->SimulationResultsExist)
    {
        emit repaintRequested();
        return;
    }

    const uint32_t bufferSize = m_MaxNumArrowsPerDrawcall * VECTOR_FIELD_3D_UBO_ELEMENT_SIZE;

    uint8_t* buffer = (uint8_t*)malloc(bufferSize);

    int i = 0;
    for (int x = 0; x < m_Simulator->SimulationNumDatapointsX; x++)
        for (int y = 0; y < m_Simulator->SimulationNumDatapointsY; y++)
            for (int z = 0; z < m_Simulator->SimulationNumDatapointsZ; z++)
            {
                QVector3D position = m_Simulator->GetPosition(x, y, z);
                QQuaternion quat = QQuaternion::fromDirection(m_Simulator->GetResult(x, y, z).normalized(), {0.0f, 0.0f, 1.0f});
                QMatrix4x4 mat;
                mat.translate(position);
                mat.rotate(quat);
                mat.scale(0.5f * std::clamp(m_Simulator->GetResult(x, y, z).length(), 0.01f, 0.2f));
                memcpy(buffer + i * sizeof(float) * 16, mat.data(), sizeof(float) * 16);

                float colorInterpolation = (m_Simulator->GetResult(x, y, z).length() - m_Simulator->SimulationResultsMinMagnitude) / (m_Simulator->SimulationResultsMaxMagnitude - m_Simulator->SimulationResultsMinMagnitude);
                QVector3D colStart = QVector3D(0.0,0.0, 1.0);
                QVector3D colEnd = QVector3D(1.0, 0.0, 0.0);
                QVector3D col = (colEnd - colStart) * colorInterpolation + colStart;
                QVector4D color(col, 1.0f);

                memcpy(buffer + m_MaxNumArrowsPerDrawcall * sizeof(float) * 16 + i * sizeof(float) * 4, &color, sizeof(float) * 4);

                i++;
            }

    m_Shader.bind();
    m_GLFuncs->glBindBuffer(GL_UNIFORM_BUFFER, m_UniformBuffer);
    m_GLFuncs->glBufferData(GL_UNIFORM_BUFFER, bufferSize, buffer, GL_DYNAMIC_DRAW);
    m_GLFuncs->glBindBuffer(GL_UNIFORM_BUFFER, 0);

    m_Shader.release();

    free(buffer);

    emit repaintRequested();
}

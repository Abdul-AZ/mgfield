#include "vectorfield3d.h"

#include <QFile>
#include <QIODevice>
#include <thirdparty/tinygltf/tiny_gltf.h>

VectorField3D::VectorField3D(QOpenGLFunctions_3_3_Core* funcs)
    : m_GLFuncs(funcs),
    m_VertexBuffer(QOpenGLBuffer::VertexBuffer),
    m_IndexBuffer(QOpenGLBuffer::IndexBuffer)
{
    m_Shader.create();

    if(!m_Shader.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/VectorField3D.vert"))
        qCritical() << "VectorField3D vertex shader error";

    if(!m_Shader.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/VectorField3D.frag"))
        qCritical() << "VectorField3D fragment shader error";

    if(!m_Shader.link())
        qCritical() << "VectorField3D shader program linking error";

    m_VertexArray.create();
    m_VertexArray.bind();

    loadModel();

    m_Shader.bind();
    m_Shader.setAttributeBuffer(0, GL_FLOAT, 0, 3, 0);
    m_Shader.enableAttributeArray(0);
}

void VectorField3D::loadModel()
{
    using namespace tinygltf;
    Model model;
    TinyGLTF loader;
    std::string err;
    std::string warn;

    QFile file(":/res/shapes/Arrow3D.glb");
    file.open(QIODevice::ReadOnly);
    auto data = file.readAll();
    file.close();

    loader.LoadBinaryFromMemory(&model, &err, &warn, (uint8_t*)data.data(), data.length());

    if((model.meshes.size() != 1) || model.bufferViews.size() != 2)
        throw std::runtime_error("Invalid meshes in 3D arrow model");

    bool foundVertexData = false;
    for (auto& bufferView : model.bufferViews)
        if(bufferView.target == GL_ARRAY_BUFFER)
        {
            foundVertexData = true;

            m_VertexBuffer.create();
            m_VertexBuffer.bind();
            m_VertexBuffer.allocate(model.buffers.at(bufferView.buffer).data.data() + bufferView.byteOffset, bufferView.byteLength);

            break;
        }
    if(!foundVertexData)
        throw std::runtime_error("Invalid vertex buffer in 3D arrow model");


    bool foundIndexData = false;
    for (auto& bufferView : model.bufferViews)
        if(bufferView.target == GL_ELEMENT_ARRAY_BUFFER)
        {
            foundIndexData = true;

            m_IndexBuffer.create();
            m_IndexBuffer.bind();
            m_IndexBuffer.allocate(model.buffers.at(bufferView.buffer).data.data() + bufferView.byteOffset, bufferView.byteLength);
            m_NumIndecies = m_IndexBuffer.size() / sizeof(uint16_t);

            break;
        }
    if(!foundIndexData)
        throw std::runtime_error("Invalid index buffer in 3D arrow model");
}

void VectorField3D::Draw(QMatrix4x4 viewProjection)
{
    m_Shader.bind();
    m_VertexArray.bind();

    QMatrix4x4 matrix;
    matrix.scale(0.1f);

    m_Shader.setUniformValue(m_Shader.uniformLocation("uModelViewProjection"), viewProjection);

    m_GLFuncs->glDrawElements(GL_TRIANGLES, m_NumIndecies, GL_UNSIGNED_SHORT, 0);

    m_Shader.release();
    m_VertexArray.release();
}
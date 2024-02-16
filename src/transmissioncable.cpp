#include "transmissioncable.h"

#include <QDebug>
#include <QFile>
#include <thirdparty/tinygltf/tiny_gltf.h>

TransmissionCable::TransmissionCable(QOpenGLFunctions_3_3_Core* funcs)
    :
    Object(CurrentCarryingCable),
    m_VertexBuffer(QOpenGLBuffer::VertexBuffer),
    m_IndexBuffer(QOpenGLBuffer::IndexBuffer)
{
    Name = "Current Carrying Conductor";

    if(!m_Shader.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/ColoredMesh.vert"))
        qCritical() << "TransmissionLine vertex shader error";

    if(!m_Shader.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/ColoredMesh.frag"))
        qCritical() << "TransmissionLine fragment shader error";

    if(!m_Shader.link())
        qCritical() << "TransmissionLine shader program linking error";

    m_VertexArray.create();
    m_VertexArray.bind();

    loadModel();

    m_Shader.bind();
    m_Shader.setAttributeBuffer(0, GL_FLOAT, 0, 3, 0);
    m_Shader.enableAttributeArray(0);
    m_Shader.setUniformValue("uMeshColor", QVector4D(1.0f, 1.0f, 1.0f, 1.0f));

    m_Shader.release();
    m_VertexArray.release();
}

void TransmissionCable::Draw(QMatrix4x4 viewProjection, QOpenGLFunctions_3_3_Core* funcs)
{
    m_VertexArray.bind();
    m_Shader.bind();

    QMatrix4x4 matrix;
    matrix *= viewProjection;

    matrix.translate(Position);
    matrix.rotate(Rotation);
    matrix.scale(0.1f);

    m_Shader.setUniformValue(m_Shader.uniformLocation("uModelViewProjection"), matrix);

    funcs->glDrawElements(GL_TRIANGLES, m_NumIndecies, GL_UNSIGNED_SHORT, 0);

    m_Shader.release();
    m_VertexArray.release();
}

void TransmissionCable::loadModel()
{
    using namespace tinygltf;
    Model model;
    TinyGLTF loader;
    std::string err;
    std::string warn;

    QFile file(":/res/shapes/Cable3D.glb");
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

            std::vector<QVector3D> vertices;
            vertices.resize(bufferView.byteLength / sizeof(QVector3D));

            memcpy(vertices.data(), model.buffers.at(bufferView.buffer).data.data() + bufferView.byteOffset, bufferView.byteLength);
            bool needsScaling = model.nodes[0].scale.size() > 0;
            if(needsScaling)
            {
                std::vector<double>& scaleData = model.nodes[0].scale;
                QVector3D scale (scaleData[0], scaleData[1], scaleData[2]);
                for(QVector3D& vec : vertices)
                    vec = vec * scale;
            }

            bool needsRotating = model.nodes[0].rotation.size() > 0;
            if(needsRotating)
            {
                std::vector<double>& rotationData = model.nodes[0].rotation; //quat
                QQuaternion rot (rotationData[0], rotationData[1], rotationData[2], rotationData[3]);
                for(QVector3D& vec : vertices)
                    vec = rot * vec;
            }

            m_VertexBuffer.create();
            m_VertexBuffer.bind();
            m_VertexBuffer.allocate(vertices.data(), bufferView.byteLength);

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

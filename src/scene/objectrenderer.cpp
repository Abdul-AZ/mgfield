#include "objectrenderer.h"

#include "thirdparty/tinygltf/tiny_gltf.h"
#include <QFile>

ObjectRenderer::ObjectRenderer()
{
    InitCableRendering();
}

ObjectRenderer::~ObjectRenderer()
{
    TerminateCableRendering();
}

void ObjectRenderer::LoadModel(QString path, QOpenGLBuffer& vbuffer, QOpenGLBuffer& ibuffer, int32_t& numIndecies)
{
    using namespace tinygltf;
    Model model;
    TinyGLTF loader;
    std::string err;
    std::string warn;

    QFile file(path);
    file.open(QIODevice::ReadOnly);
    auto data = file.readAll();
    file.close();

    loader.LoadBinaryFromMemory(&model, &err, &warn, (uint8_t*)data.data(), data.length());

    if((model.meshes.size() != 1) || model.bufferViews.size() != 2)
        throw std::runtime_error("Invalid mesh");

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

            vbuffer.create();
            vbuffer.bind();
            vbuffer.allocate(vertices.data(), bufferView.byteLength);

            break;
        }
    if(!foundVertexData)
        throw std::runtime_error("Invalid vertex buffer in model");


    bool foundIndexData = false;
    for (auto& bufferView : model.bufferViews)
        if(bufferView.target == GL_ELEMENT_ARRAY_BUFFER)
        {
            foundIndexData = true;

            ibuffer.create();
            ibuffer.bind();
            ibuffer.allocate(model.buffers.at(bufferView.buffer).data.data() + bufferView.byteOffset, bufferView.byteLength);
            numIndecies = ibuffer.size() / sizeof(uint16_t);

            break;
        }
    if(!foundIndexData)
        throw std::runtime_error("Invalid index buffer in model");
}

void ObjectRenderer::InitCableRendering()
{
    m_CableVertexBuffer = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    m_CableIndexBuffer = QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);

    if(!m_CableShader.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/ColoredMesh.vert"))
        qCritical() << "TransmissionLine vertex shader error";

    if(!m_CableShader.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/ColoredMesh.frag"))
        qCritical() << "TransmissionLine fragment shader error";

    if(!m_CableShader.link())
        qCritical() << "TransmissionLine shader program linking error";

    m_CableVertexArray.create();
    m_CableVertexArray.bind();

    LoadModel(":/res/shapes/Cable3D.glb", m_CableVertexBuffer, m_CableIndexBuffer, m_CableNumIndecies);

    m_CableShader.bind();
    m_CableShader.setAttributeBuffer(0, GL_FLOAT, 0, 3, 0);
    m_CableShader.enableAttributeArray(0);
    m_CableShader.setUniformValue("uMeshColor", QVector4D(1.0f, 1.0f, 1.0f, 1.0f));

    m_CableShader.release();
    m_CableVertexArray.release();
}

void ObjectRenderer::TerminateCableRendering()
{
    m_CableVertexArray.destroy();
    m_CableVertexBuffer.destroy();
    m_CableIndexBuffer.destroy();
}

void ObjectRenderer::DrawCable(TransmissionCable* object, QOpenGLFunctions* funcs, const QMatrix4x4& viewProjection)
{
    m_CableVertexArray.bind();
    m_CableShader.bind();

    QMatrix4x4 matrix;
    matrix *= viewProjection;

    matrix.translate(object->Position);
    matrix.rotate(object->Rotation);
    if(object->GetIsInfiniteLength())
        matrix.scale(1000.0f, TRANSMISSION_CABLE_MODEL_BASE_SCALE, TRANSMISSION_CABLE_MODEL_BASE_SCALE);
    else
        matrix.scale(TRANSMISSION_CABLE_MODEL_BASE_SCALE);

    m_CableShader.setUniformValue(m_CableShader.uniformLocation("uModelViewProjection"), matrix);

    funcs->glDrawElements(GL_TRIANGLES, m_CableNumIndecies, GL_UNSIGNED_SHORT, 0);

    m_CableShader.release();
    m_CableVertexArray.release();
}

void ObjectRenderer::DrawScene(QOpenGLContext* context, Scene* scene, const QMatrix4x4& viewProjection)
{
    if(QOpenGLContext::currentContext() != context)
        context->makeCurrent(nullptr);

    for (auto& object : scene->Objects)
    {
        if(object->Type == CurrentCarryingCable)
        {
            DrawCable((TransmissionCable*)object.get(), context->functions(), viewProjection);
        }
    }
}

#include "objectrenderer.h"

#include "thirdparty/tinygltf/tiny_gltf.h"
#include <QFile>

ObjectRenderer::ObjectRenderer()
{
    LoadShaders();
    InitCableRendering();
    InitSheetRendering();
}

ObjectRenderer::~ObjectRenderer()
{
    TerminateCableRendering();
    TerminateSheetRendering();
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

void ObjectRenderer::LoadShaders()
{
    if(!m_ColoredMeshShader.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/ColoredMesh.vert"))
        qCritical() << "ColoredMesh vertex shader error";

    if(!m_ColoredMeshShader.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/ColoredMesh.frag"))
        qCritical() << "ColoredMesh fragment shader error";

    if(!m_ColoredMeshShader.link())
        qCritical() << "ColoredMesh shader program linking error";

    m_ColoredMeshShader.bind();
    m_ColoredMeshShader.setUniformValue("uMeshColor", QVector4D(1.0f, 1.0f, 1.0f, 1.0f));
    m_ColoredMeshShader.release();
}

void ObjectRenderer::InitCableRendering()
{
    m_CableModelData.VertexBuffer = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    m_CableModelData.IndexBuffer = QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);

    m_CableModelData.VertexArray.create();
    m_CableModelData.VertexArray.bind();

    LoadModel(":/res/shapes/Cable3D.glb", m_CableModelData.VertexBuffer, m_CableModelData.IndexBuffer, m_CableModelData.NumIndecies);

    m_ColoredMeshShader.setAttributeBuffer(0, GL_FLOAT, 0, 3, 0);
    m_ColoredMeshShader.enableAttributeArray(0);

    m_CableModelData.VertexArray.release();
}

void ObjectRenderer::TerminateCableRendering()
{
    m_CableModelData.VertexArray.destroy();
    m_CableModelData.VertexBuffer.destroy();
    m_CableModelData.IndexBuffer.destroy();
}

void ObjectRenderer::DrawCable(StraightWireObject* object, QOpenGLFunctions* funcs, const QMatrix4x4& viewProjection)
{
    m_CableModelData.VertexArray.bind();
    m_ColoredMeshShader.bind();

    QMatrix4x4 matrix;
    matrix *= viewProjection;

    matrix.translate(object->Position);
    matrix.rotate(object->Rotation);
    if(object->GetIsInfiniteLength())
        matrix.scale(1000.0f, STRAIGHT_WIRE_OBJECT_MODEL_BASE_SCALE, STRAIGHT_WIRE_OBJECT_MODEL_BASE_SCALE);
    else
        matrix.scale(STRAIGHT_WIRE_OBJECT_MODEL_BASE_SCALE);

    m_ColoredMeshShader.setUniformValue(m_ColoredMeshShader.uniformLocation("uModelViewProjection"), matrix);

    funcs->glDrawElements(GL_TRIANGLES, m_CableModelData.NumIndecies, GL_UNSIGNED_SHORT, 0);

    m_ColoredMeshShader.release();
    m_CableModelData.VertexArray.release();
}

void ObjectRenderer::DrawScene(QOpenGLContext* context, Scene* scene, const QMatrix4x4& viewProjection)
{
    if(QOpenGLContext::currentContext() != context)
        context->makeCurrent(nullptr);

    for (auto& object : scene->Objects)
    {
        switch (object->Type)
        {
        case ObjectType::StraightWire:
            DrawCable((StraightWireObject*)object.get(), context->functions(), viewProjection);
            continue;

        case ObjectType::CurrentCarryingSheet:
            DrawSheet((CurrentCarryingSheet*)object.get(), context->functions(), viewProjection);
            continue;

        default:
            qWarning("Tried to draw unknown object");
            continue;
        }
    }
}

void ObjectRenderer::InitSheetRendering()
{
    m_SheetModelData.VertexBuffer = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    m_SheetModelData.IndexBuffer = QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);

    m_SheetModelData.VertexArray.create();
    m_SheetModelData.VertexArray.bind();

    LoadModel(":/res/shapes/Plane3D.glb", m_SheetModelData.VertexBuffer, m_SheetModelData.IndexBuffer, m_SheetModelData.NumIndecies);

    m_ColoredMeshShader.setAttributeBuffer(0, GL_FLOAT, 0, 3, 0);
    m_ColoredMeshShader.enableAttributeArray(0);

    m_SheetModelData.VertexArray.release();
}

void ObjectRenderer::TerminateSheetRendering()
{
    m_SheetModelData.VertexArray.destroy();
    m_SheetModelData.VertexBuffer.destroy();
    m_SheetModelData.IndexBuffer.destroy();
}

void ObjectRenderer::DrawSheet(CurrentCarryingSheet* object, QOpenGLFunctions* funcs, const QMatrix4x4& viewProjection)
{
    m_SheetModelData.VertexArray.bind();
    m_ColoredMeshShader.bind();

    QMatrix4x4 matrix;
    matrix *= viewProjection;

    matrix.scale(object->GetLength(), 1.0f, object->GetWidth());
    matrix.rotate(object->Rotation);
    matrix.translate(object->Position);

    m_ColoredMeshShader.setUniformValue(m_ColoredMeshShader.uniformLocation("uModelViewProjection"), matrix);

    funcs->glDrawElements(GL_TRIANGLES, m_SheetModelData.NumIndecies, GL_UNSIGNED_SHORT, 0);

    m_ColoredMeshShader.release();
    m_SheetModelData.VertexArray.release();
}

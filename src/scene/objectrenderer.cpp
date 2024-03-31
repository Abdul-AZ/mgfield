#include "objectrenderer.h"

#include "src/modelloader.h"

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
    ModelLoader::SingleMeshData mesh = ModelLoader::LoadSingleMeshFile(path);
    vbuffer.create();
    vbuffer.bind();
    vbuffer.allocate(mesh.Vertices.data(), mesh.Vertices.size() * sizeof(QVector3D));

    ibuffer.create();
    ibuffer.bind();
    ibuffer.allocate(mesh.Indecies.data(), mesh.Indecies.size() * sizeof(uint16_t));
    numIndecies = ibuffer.size() / sizeof(uint16_t);
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

    uint8_t stencilBufferValue = 0;
    for (auto& object : scene->Objects)
    {
        context->functions()->glStencilFunc(GL_ALWAYS, stencilBufferValue, 0xFF);

        switch (object->Type)
        {
        case ObjectType::StraightWire:
            DrawCable((StraightWireObject*)object.get(), context->functions(), viewProjection);
            break;

        case ObjectType::CurrentCarryingSheet:
            DrawSheet((CurrentCarryingSheet*)object.get(), context->functions(), viewProjection);
            break;

        default:
            qWarning("Tried to draw unknown object");
            break;
        }

        stencilBufferValue++;
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

    matrix.translate(object->Position);
    matrix.rotate(object->Rotation);
    matrix.scale(object->GetLength(), 1.0f, object->GetWidth());

    m_ColoredMeshShader.setUniformValue(m_ColoredMeshShader.uniformLocation("uModelViewProjection"), matrix);

    funcs->glDrawElements(GL_TRIANGLES, m_SheetModelData.NumIndecies, GL_UNSIGNED_SHORT, 0);

    m_ColoredMeshShader.release();
    m_SheetModelData.VertexArray.release();
}

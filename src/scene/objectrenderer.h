#ifndef OBJECTRENDERER_H
#define OBJECTRENDERER_H

#include "src/scene.h"

#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLFunctions>

#include "src/scene/straightwireobject.h"

class ObjectRenderer
{
public:
    ObjectRenderer();
    ~ObjectRenderer();

    void DrawScene(QOpenGLContext* context, Scene* scene, const QMatrix4x4& viewProjection);

private:
    void InitCableRendering();
    void TerminateCableRendering();
    void DrawCable(StraightWireObject* object, QOpenGLFunctions* funcs, const QMatrix4x4& viewProjection);

    void LoadModel(QString path, QOpenGLBuffer& vbuffer, QOpenGLBuffer& ibuffer, int32_t& numIndecies);

    void LoadShaders();

private:
    QOpenGLShaderProgram m_ColoredMeshShader;

    struct ObjectTypeRenderData
    {
        QOpenGLBuffer              VertexBuffer;
        QOpenGLBuffer              IndexBuffer;
        QOpenGLVertexArrayObject   VertexArray;
        int32_t                    NumIndecies;
    };

    ObjectTypeRenderData m_CableModelData;
    ObjectTypeRenderData m_SheetModelData;
};

#endif // OBJECTRENDERER_H

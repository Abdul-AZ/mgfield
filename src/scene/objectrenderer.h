#ifndef OBJECTRENDERER_H
#define OBJECTRENDERER_H

#include "src/scene.h"
#include "src/vectorfield3d.h"

#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLFunctions>

#include "src/scene/straightwireobject.h"
#include "src/scene/currentcarryingsheet.h"

#define OBJECT_RENDERER_NORMAL_VECTOR_COLOR  QVector4D(0.0f,1.0f,0.0f,1.0f)
#define OBJECT_RENDERER_NORMAL_VECTOR_SCALE  (0.05f)
#define OBJECT_RENDERER_CURRENT_VECTOR_COLOR QVector4D(0.0f,1.0f,1.0f,1.0f)
#define OBJECT_RENDERER_CURRENT_VECTOR_SCALE (0.05f)

class ObjectRenderer
{
public:
    ObjectRenderer();
    ~ObjectRenderer();

    void DrawScene(QOpenGLContext* context, Scene* scene, VectorField3D* arrowRenderer, const QMatrix4x4& viewProjection);

private:
    void InitCableRendering();
    void TerminateCableRendering();
    void DrawCable(StraightWireObject* object, QOpenGLFunctions* funcs, const QMatrix4x4& viewProjection);

    void InitSheetRendering();
    void TerminateSheetRendering();
    void DrawSheet(CurrentCarryingSheet* object, QOpenGLFunctions* funcs, const QMatrix4x4& viewProjection);

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

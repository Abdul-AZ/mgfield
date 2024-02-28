#ifndef OBJECTRENDERER_H
#define OBJECTRENDERER_H

#include "src/scene.h"

#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLFunctions>

#include "src/transmissioncable.h"

class ObjectRenderer
{
public:
    ObjectRenderer();
    ~ObjectRenderer();

    void DrawScene(QOpenGLContext* context, Scene* scene, const QMatrix4x4& viewProjection);

private:
    void InitCableRendering();
    void TerminateCableRendering();
    void DrawCable(TransmissionCable* object, QOpenGLFunctions* funcs, const QMatrix4x4& viewProjection);

    void LoadModel(QString path, QOpenGLBuffer& vbuffer, QOpenGLBuffer& ibuffer, int32_t& numIndecies);

private:
    QOpenGLShaderProgram       m_CableShader;
    QOpenGLBuffer              m_CableVertexBuffer;
    QOpenGLBuffer              m_CableIndexBuffer;
    QOpenGLVertexArrayObject   m_CableVertexArray;
    int32_t                    m_CableNumIndecies;
};

#endif // OBJECTRENDERER_H

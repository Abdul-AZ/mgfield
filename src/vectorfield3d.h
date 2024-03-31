#ifndef VECTORFIELD3D_H
#define VECTORFIELD3D_H

#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include "src/sim/mfsimulator.h"
#include <QObject>

#define VECTOR_FIELD_3D_MAX_ARROW_COUNT  (32768)
#define VECTOR_FIELD_3D_ARROW_MODEL_PATH ":/res/shapes/Arrow3D.glb"

class VectorField3D : public QObject
{
    Q_OBJECT

public:
    VectorField3D(QOpenGLFunctions_3_3_Core* funcs);
    virtual ~VectorField3D() {};

    void Draw(QMatrix4x4 viewProjection);

public slots:
    void updateBuffers();

signals:
    void repaintRequested();

private:
    void loadModel();

private:
    QOpenGLFunctions_3_3_Core* m_GLFuncs;
    QOpenGLVertexArrayObject   m_VertexArray;
    QOpenGLBuffer              m_VertexBuffer;
    QOpenGLBuffer              m_IndexBuffer;
    uint32_t                   m_UniformBuffer;
    QOpenGLShaderProgram       m_Shader;
    int32_t                    m_NumIndecies;
    MFSimulator*               m_Simulator;
};

#endif // VECTORFIELD3D_H

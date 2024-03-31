#ifndef VECTORFIELD3D_H
#define VECTORFIELD3D_H

#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include "src/sim/mfsimulator.h"
#include <QObject>

#define VECTOR_FIELD_3D_ARROW_MODEL_PATH    ":/res/shapes/Arrow3D.glb"
#define VECTOR_FIELD_3D_UBO_ELEMENT_SIZE    (20 * sizeof(float))

/** Size of uniform buffer if maximum allowed size in device is large enough (hard cap) */
#define VECTOR_FIELD_3D_UBO_MAX_WANTED_SIZE (8192 * VECTOR_FIELD_3D_UBO_ELEMENT_SIZE)

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
    int32_t                    m_MaxNumArrowsPerDrawcall;
};

#endif // VECTORFIELD3D_H

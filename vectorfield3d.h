#ifndef VECTORFIELD3D_H
#define VECTORFIELD3D_H

#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>

class VectorField3D
{
public:
    VectorField3D(QOpenGLFunctions_3_3_Core* funcs);

    void Draw(QMatrix4x4 viewProjection);

private:
    void loadModel();

private:
    QOpenGLFunctions_3_3_Core* m_GLFuncs;
    QOpenGLVertexArrayObject   m_VertexArray;
    QOpenGLBuffer              m_VertexBuffer;
    QOpenGLBuffer              m_IndexBuffer;
    QOpenGLShaderProgram       m_Shader;
    int32_t                    m_NumIndecies;
};

#endif // VECTORFIELD3D_H

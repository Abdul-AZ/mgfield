#ifndef GRID3D_H
#define GRID3D_H

#include <QObject>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>

// This is the number of lines in one axis
// NOTE: must be a positive odd number
#define GRID3D_NUMBER_LINES     (101)
#define GRID3D_NUMBER_VERTICES  (4 * GRID3D_NUMBER_LINES)

class Grid3D : public QObject
{
    Q_OBJECT

public:
    Grid3D(QOpenGLFunctions_3_3_Core* funcs);

    void Draw(const QMatrix4x4& viewProjection);

private:
    void createBuffers();

private:
    QOpenGLFunctions_3_3_Core* m_GLFuncs;
    QOpenGLShaderProgram       m_Shader;
    QOpenGLVertexArrayObject   m_VertexArray;
    QOpenGLBuffer              m_VertexBuffer;
};

#endif // GRID3D_H

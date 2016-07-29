#ifndef CUBEPIECE_H
#define CUBEPIECE_H

#include <QMessageBox>
#include <QTextStream>
#include <QtOpenGL>
#include <QOpenGLWidget>
#include <QFile>
#include <QGLShaderProgram>
#include <QOpenGLFunctions>

#define CUBEPIECE_RIGHT_FACE  0
#define CUBEPIECE_LEFT_FACE   1
#define CUBEPIECE_UP_FACE     2
#define CUBEPIECE_DOWN_FACE   3
#define CUBEPIECE_FRONT_FACE  4
#define CUBEPIECE_BACK_FACE   5

#define CUBEPIECE_COLOR_WHITE   QVector4D(1, 1, 1, 1)
#define CUBEPIECE_COLOR_BLACK   QVector4D(0, 0, 0, 1)
#define CUBEPIECE_COLOR_RED     QVector4D(1, 0, 0, 1)
#define CUBEPIECE_COLOR_GREEN   QVector4D(0, 1, 0, 1)
#define CUBEPIECE_COLOR_BLUE    QVector4D(0, 0, 1, 1)
#define CUBEPIECE_COLOR_YELLOW  QVector4D(0, 1, 1, 1)
#define CUBEPIECE_COLOR_ORANGE  QVector4D(1, 1, 0, 1)
#define CUBEPIECE_COLOR_GRAY    QVector4D(0.3, 0.3, 0.3, 1)

class CubePiece : protected QOpenGLFunctions
{
public:
    CubePiece(QGLWidget *widget, float distance, int x, int y, int z);
    ~CubePiece();
    void drawObject();
    void rotate(float angle, const QVector3D &vec);
    void translate(const QVector3D &vector);
    void set_pMatrix(QMatrix4x4 pMatrix) { this->pMatrix = pMatrix; }
    void set_vMatrix(QMatrix4x4 vMatrix) { this->vMatrix = vMatrix; }
    int idxx() { return this->m_idxx; }
    int idxy() { return this->m_idxy; }
    int idxz() { return this->m_idxz; }
    int setidx(int idxx, int idxy, int idxz);

protected:
    void createVBOs();
    void destroyVBOs();
    void calcVerticesNormal();
    void readOFFFile(QFile *file);

    QGLWidget *widget;
    QMatrix4x4 pMatrix;
    QMatrix4x4 vMatrix;

    QGLShader *vertexShader;
    QGLShader *fragmentShader;
    QGLShaderProgram *sProgram;

    QOpenGLVertexArrayObject *vao;
    QOpenGLBuffer *vboVertices;
    QOpenGLBuffer *vboIndices;
    QOpenGLBuffer *vboNormals;
    QVector4D *vertices;
    QVector3D *normals;
    QVector3D *color;
    unsigned int* indices;
    unsigned int vnum, fnum, inum;

    int m_idxx, m_idxy, m_idxz;
    QVector4D rFaceVertices[4];
    QVector4D lFaceVertices[4];
    QVector4D uFaceVertices[4];
    QVector4D dFaceVertices[4];
    QVector4D fFaceVertices[4];
    QVector4D bFaceVertices[4];
};

#endif // CUBEPIECE_H

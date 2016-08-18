#ifndef CUBEPIECE_H
#define CUBEPIECE_H

#include <QMessageBox>
#include <QTextStream>
#include <QtOpenGL>
#include <QOpenGLWidget>
#include <QFile>
#include <QGLShaderProgram>
#include <QOpenGLFunctions>
#include "light.h"
#include "material.h"

class CubePiece : protected QOpenGLFunctions
{
public:
    CubePiece(QGLWidget *widget, float distance, int x, int y, int z, Light light, Material material);
    ~CubePiece();
    void drawObject();
    void rotate(float angle, const QVector3D &vec);
    void translate(const QVector3D &vector);
    void set_pMatrix(QMatrix4x4 pMatrix) { this->pMatrix = pMatrix; }
    void set_vMatrix(QMatrix4x4 vMatrix) { this->vMatrix = vMatrix; }
    QVector3D initialPosition() { return QVector3D(i_idxx, i_idxy, i_idxz); }
    QVector3D currentPosition() { return QVector3D(m_idxx, m_idxy, m_idxz); }
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
    QMatrix4x4 nMatrix;

    Light light;
    Material material;

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
    int i_idxx, i_idxy, i_idxz;
    QVector4D rFaceVertices[4];
    QVector4D lFaceVertices[4];
    QVector4D uFaceVertices[4];
    QVector4D dFaceVertices[4];
    QVector4D fFaceVertices[4];
    QVector4D bFaceVertices[4];
};

#endif // CUBEPIECE_H

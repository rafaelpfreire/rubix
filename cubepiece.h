#ifndef CUBEPIECE_H
#define CUBEPIECE_H

#include <QMessageBox>
#include <QTextStream>
#include <QtOpenGL>
#include <QOpenGLWidget>
#include <QFile>
#include <QGLShaderProgram>
#include <QOpenGLFunctions>

class CubePiece : protected QOpenGLFunctions
{
public:
    CubePiece(QGLWidget *widget, float distance, int x, int y, int z);
    ~CubePiece();
    void drawObject();
    void rotate(float anglex, float angley, float anglez);
    void translate(QVector3D &vector);
    void translate(float x, float y, float z);
    void set_pMatrix(QMatrix4x4 pMatrix) { this->pMatrix = pMatrix; }
    void set_vMatrix(QMatrix4x4 vMatrix) { this->vMatrix = vMatrix; }
    int idxx() { return this->m_idxx; }
    int idxy() { return this->m_idxy; }
    int idxz() { return this->m_idxz; }

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
    QVector3D *vertices;
    QVector3D *normals;
    QVector3D *color;
    unsigned int* indices;
    unsigned int vnum, fnum, inum;

    QVector4D position;
    QVector3D angle;
    int m_idxx, m_idxy, m_idxz;
};

#endif // CUBEPIECE_H

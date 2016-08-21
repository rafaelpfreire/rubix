#ifndef GRAPHICOBJECT_H
#define GRAPHICOBJECT_H

#include <QtOpenGL>
#include <QOpenGLWidget>
#include <QFile>
#include <QGLShaderProgram>
#include <QOpenGLFunctions>
#include <QMessageBox>
#include <QTextStream>

class GraphicObject : protected QOpenGLFunctions
{
public:
    GraphicObject(QGLWidget *widget, const QString vShaderFile, const QString fShaderFile,
                  const QString offFileName);
    ~GraphicObject();
    virtual void drawObject();

protected:
    void createVBOs();
    void destroyVBOs();
    void calcVerticesNormal();
    void readOFFFile(QFile *file);

    QGLWidget *widget;

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

};

#endif // GRAPHICOBJECT_H

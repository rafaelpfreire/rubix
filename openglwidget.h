#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QFileDialog>
#include <QGLWidget>
#include <QGLShaderProgram>
#include <QMouseEvent>
#include "rubixcube.h"

class OpenGLWidget : public QGLWidget
{
    Q_OBJECT

public:
    explicit OpenGLWidget();
    ~OpenGLWidget();

protected:
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

private:
//    CubePiece *globj, *globj2;
    RubixCube *rubix;
    QMatrix4x4 pMatrix;
    QGLShaderProgram sProgram;
    QVector<QVector3D> vertices;
    QVector<QVector3D> color;
    double alpha;
    double beta;
    double gama;
    double distance;
    QPoint lastMousePosition;
};

#endif // OPENGLWIDGET_H

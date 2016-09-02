#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QGLWidget>
#include <QGLShaderProgram>
#include <QMouseEvent>
#include <QKeyEvent>
#include <cmath>
#include <QTimer>
#include "rubixcube.h"
#include "stopwatch.h"
#include "material.h"
#include "camera.h"
#include "light.h"

class OpenGLWidget : public QGLWidget
{
    Q_OBJECT

public:
    explicit OpenGLWidget(QStatusBar *parent);
    ~OpenGLWidget();

    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);

protected:
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();
    void keyPressEvent(QKeyEvent *event);

    bool dragging = false, isPlaying = false;
    int xant, yant, dx, dy;

protected slots:
    void killMovement();

private:
    Light *light;
    Material *material;
    Camera *camera;

    QTimer* timer;
    RubixCube *rubix;
    StopWatch *stopWatch;
    QGLShaderProgram sProgram;
    QVector<QVector3D> vertices;
    QVector<QVector3D> color;
    double alpha;
    double beta;
    double gama;
    double distance;
    QPoint lastMousePosition;

public slots:
    void shuffleCube();

protected slots:
    void cubeSolvedCallback();
    void shuffleEndCallback();
};

#endif // OPENGLWIDGET_H

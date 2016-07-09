#include "openglwidget.h"

OpenGLWidget::OpenGLWidget()
{
    alpha = 30;
    beta = -30;
    gama = 0;
    distance = 3;

    rubix = new RubixCube(this);
}

OpenGLWidget::~OpenGLWidget()
{
    if( rubix ) {
        delete rubix;
        rubix = NULL;
    }
}

void OpenGLWidget::initializeGL()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    qglClearColor(QColor(Qt::black));
}

void OpenGLWidget::resizeGL(int width, int height)
{
    if (height == 0) {
        height = 1;
    }

    // Projection Matrix initialized via "perspective" function
    pMatrix.setToIdentity();
    pMatrix.perspective(60.0, (float) width / (float) height, 0.001, 1000);
    glViewport(0, 0, width, height);

    rubix->set_pMatrix(this->pMatrix);
}

void OpenGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QMatrix4x4 vMatrix;
    QMatrix4x4 cameraTransformation;

    // TODO: Prevent Gimbal Lock
    cameraTransformation.rotate(beta , 1, 0, 0);
    cameraTransformation.rotate(alpha, 0, 1, 0);
    cameraTransformation.rotate(gama , 0, 0, 1);

    QVector3D cameraPosition = cameraTransformation * QVector3D(0, 0, distance);
    QVector3D cameraUpDirection = cameraTransformation * QVector3D(0, 1, 0);

    // eye:        Cameras position to real world
    // center:     Target Point in real world
    // up vector:  Where is the up direction
    vMatrix.lookAt(cameraPosition, QVector3D(0, 0, 0), cameraUpDirection);

    rubix->set_vMatrix(vMatrix);
    rubix->drawObject();
}

void OpenGLWidget::mousePressEvent(QMouseEvent *event)
{
    if( event->buttons() & Qt::LeftButton ) {
        rubix->rotate(0, 0, 15);
    }

    updateGL();
    event->accept();
}

void OpenGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int deltaX = event->x() - lastMousePosition.x();
    int deltaY = event->y() - lastMousePosition.y();

    if (event->buttons() & Qt::LeftButton) {
//        alpha -= deltaX;
//        while (alpha < 0) {
//            alpha += 360;
//        }
//        while (alpha >= 360) {
//            alpha -= 360;
//        }
//        beta -= deltaY;
//        if (beta < 0) {
//            beta += 360;
//        }
//        if (beta >= 360) {
//            beta -= 360;
//        }
        updateGL();
    }

    lastMousePosition = event->pos();
    event->accept();
}

void OpenGLWidget::wheelEvent(QWheelEvent *event)
{
    int delta = event->delta();

    if (event->orientation() == Qt::Vertical) {
        if (delta < 0) {
            distance *= 1.1;
        } else if (delta > 0) {
            distance *= 0.9;
        }
        updateGL();
    }

    event->accept();
}
